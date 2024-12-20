# Solfacon
### An efficient fan controller designed for solar powered sustainable / off-grid ventilation

<img src="enclosure_example2.jpg" width="300">

Feature highlights:
* Low-light operation / assisted fan start
* Adjustable max fan speed - PWM duty cycle calculated on the basis of available voltage and desired fan speed
* Directly connect 1 or 2 fans (designed for 3-pin or 4-pin 12V PC fans)
* LDO / switch mode combo power regulation for efficient power conditioning
* Easy assembly in standard enclosure
* Operating temperatures from -20°C to 50°C
* Easy to configure, modify and hack
* Controlled by ATtiny85 MCU

Read the details here and configure your own:
https://s0rent.github.io/solfacon/


Software used: <br>
KiCad 6.0, Arduino IDE 2.2.0, ATtiny package for Arduino IDE (https://github.com/damellis/attiny)

Hardware used: <br>
USBtinyISP (it is also possible to use for instance an Arduino as ISP)