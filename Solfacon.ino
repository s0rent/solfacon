#include "util/delay.h"  // Any delays used are off by factor ~1.3 due to interrupts caused by timer0 (PWM out) - only matters if you need exact timing
#include "fan-curve.h"

#define F_CPU 800000

/*
Physical pin numbers:
[5] : OUT fan on/off (for LED?)
[6] : OUT pwm
[7] : OUT fan on/off (high/low) 
[2] : IN  max fan speed potentiometer 1/2 -> 1/1
[3] : IN  12V rail voltage divider
*/

#define pinOutLED PB0
#define pinOutPWM PB1
#define pinOutFanEnable PB2
#define pinInTargetFanSpeed MUX3
#define pinInVoltage12VRail MUX2

const byte speedAdjustmentEnabled = true;
const byte lowPowerOperationEnabled = true;
const byte adaptiveModeEnabled = true;  // Usable when speed adjustment is not enabled or target speed = max speed.

const byte measurementsPerSecond = 64;
const byte delayBetweenMeasurements = 1000 / measurementsPerSecond;
// variables for averaging voltages in a 1/4 second window
byte latestVoltageMeasurements[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
byte voltageMeasurementIndex = 0;

bool fanOn = false;
byte fanTargetSpeed = 63;  // Target speed relative to max speed - range 0-63 (40%-88%)
byte voltage12VRail = 0;   // Voltage relative to max voltage - range 0-127
byte dutyCycle = 40;

void setup() {
  pinMode(pinOutLED, OUTPUT);
  digitalWrite(pinOutLED, LOW);

  pinMode(pinOutPWM, OUTPUT);
  pinMode(pinOutFanEnable, OUTPUT);

  digitalWrite(pinOutPWM, LOW);
  digitalWrite(pinOutFanEnable, LOW);

  cli();

  // ADC config
  ADMUX = 0 << REFS1 | 0 << REFS0;
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);

  // Timer0 configured for phase correct PWM at 25KHz
  // Since the PWM signal from the MCU controls a MOSFET that pulls the PWM pin of the fan low when the MCU pin is high, the PWM signal is inverted.
  TCCR0A = _BV(COM0B1) | _BV(COM0B0) | _BV(WGM00);
  TCCR0B = _BV(WGM02) | _BV(CS00);
  OCR0A = 160;
  setDutyCycle(160);

  sei();
  _delay_ms(1);
}

void loop() {
  measure12vRailVoltage();
  if (fanOn) {
    measureFanTargetSpeed();
    setDutyCycle(getOptimalDutyCycle());
  }
  _delay_ms(delayBetweenMeasurements);
}

void measure12vRailVoltage() {
  byte adcReading = readADC_8bit(pinInVoltage12VRail);
  voltage12VRail = adcReading - (adcReading >> 5);  // ADC readings of the voltage divider top out at ~131 for 12V, so it must be scaled down to top at 127

  if (voltage12VRail > 127) voltage12VRail = 127;  // The voltage regulators, diodes, and resistors have some margin of error which means that the measured voltage could be 12V or above

  voltageMeasurementIndex++;
  if (voltageMeasurementIndex > 15) voltageMeasurementIndex = 0;
  latestVoltageMeasurements[voltageMeasurementIndex] = voltage12VRail;

  if (voltage12VRail >= fanOnVoltageThreshold && !fanOn) {
    // Voltage is high and the fan is off -> start the fan
    enableFan();
  } else if (voltage12VRail <= fanOffVoltageThreshold && fanOn) {
    // Voltage is low and the fan is on -> stop the fan
    disableFan();
  }
}

void measureFanTargetSpeed() {
  if (!speedAdjustmentEnabled) return 63;

  byte adcReading = readADC_8bit(pinInTargetFanSpeed);
  fanTargetSpeed = adcReading >> 2;  // ADC readings of 0-255 are scaled down to 0-63

  // The ability to reach max on a potentiometer depends on the potentiometer mechanics which can be imperfect. Some cannot reach it, others reach it before the end of the turn.
  // To ensure that the highest speed is reachable, we add a little margin.
  if (fanTargetSpeed > 61) fanTargetSpeed = 63;
}

void enableFan() {
  if (lowPowerOperationEnabled) {
    _delay_ms(10000);  // Wait a bit to increase the likelihood of a good charge
  } else {
    _delay_ms(1000);
  }
  digitalWrite(pinOutLED, HIGH);
  fanOn = true;
  digitalWrite(pinOutFanEnable, HIGH);
  setDutyCycle(32);  // using 20% duty cycle - Less than the full 100% in order to minimize the surge, but enough to ensure that the fan can start.
  _delay_ms(1500);

  if (!lowPowerOperationEnabled) {
    // Without the super capacitors the voltage can drop significantly, but "freewheeling" briefly can make it recover (or at least decrease the risk of brown out) and allow normal operation
    setDutyCycle(160);
    _delay_us(10);
    digitalWrite(pinOutLED, LOW);
    digitalWrite(pinOutFanEnable, LOW);
    _delay_ms(800);
    digitalWrite(pinOutLED, HIGH);
    digitalWrite(pinOutFanEnable, HIGH);
    setDutyCycle(32);
    _delay_ms(800);
  } else {
    _delay_ms(1000);
  }
}

void disableFan() {
  digitalWrite(pinOutLED, LOW);
  fanOn = false;
  setDutyCycle(160);                   // In inverted PWM, the PWM output is now pulled low and the MOSFET is off.
  _delay_us(10);                       // Wait so that the gate of the MOSFET controlling PWM is no longer charged, and there is no connection to GND from the fan's internal pull-up resistor
  digitalWrite(pinOutFanEnable, LOW);  // Now the fan has no path to GND
  _delay_ms(4000);                     // Wait before starting over
}

void setDutyCycle(byte newDutyCycle) {
  OCR0B = newDutyCycle;
}

byte getOptimalDutyCycle() {
  if (adaptiveModeEnabled && fanTargetSpeed == 63) {
    // Max speed setting
    // There is no way to reach max speed at less than full voltage, so there is no ramp up/down noise concern here
    // Instead, we enter "Adaptive mode" where we try to adjust the duty cycle to maintain operation above fanOnVoltageThreshold, since lower duty cycle is more efficient than letting the voltage drop
    // Fan speed might fluctuate wildly, but it there is a bigger chance that the fan will keep running, and it will run as fast as possible
    byte averageVoltage = getlatestVoltageMeasurementsAverage();
    if (voltage12VRail > fanOnVoltageThreshold) {
      if (averageVoltage > voltage12VRail) {  // below average
        decreseDutyCycle();
        decreseDutyCycle();
      } else {  // above or equal to average
        increaseDutyCycle();
      }
    } else {
      decreseDutyCycle();
      decreseDutyCycle();
    }
  } else if (voltage12VRail >= minimumVoltageForPWM) {
    // Fixed max speed
    // Try to maintain the speed no matter the circumstances
    dutyCycle = pgm_read_byte(&dutyCycleTable[voltage12VRail - minimumVoltageForPWM][fanTargetSpeed]);
  } else {
    dutyCycle = 160;  // default to max duty cycle = PWM off
  }
  return dutyCycle;
}

byte getlatestVoltageMeasurementsAverage() {
  int average = 0;
  for (byte i = 0; i < 16; i++) {
    average += latestVoltageMeasurements[i];
  }
  return average >> 4;
}

void increaseDutyCycle() {
  if (dutyCycle < 160) {
    dutyCycle++;
  }
}

void decreseDutyCycle() {
  if (dutyCycle > 32) {
    dutyCycle--;
  }
}

byte readADC_8bit(byte ADmuxPin) {
  // Disables interrupts (and PWM) to give the ADC some quiet time to do its thing.
  // Leaves the PWM output hanging in which ever state it is in.
  cli();
  ADMUX = (ADMUX & 0xF0) + ADmuxPin;
  _delay_us(10);
  ADCSRA |= 1 << ADSC;
  while (ADCSRA & _BV(ADSC))
    ;
  sei();
  return (ADC >> 2) & 0xFF;
}
