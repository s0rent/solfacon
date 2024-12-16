const byte fanOnVoltageThreshold = 117;
const byte fanOffVoltageThreshold = 43;
const byte minimumVoltageForPWM = 43;

// Table to look up duty cycle for target RPM at available voltage.
const byte dutyCycleTable[85][64] PROGMEM = {
    {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {143, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {138, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {134, 140, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {130, 136, 142, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {127, 132, 138, 144, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {124, 129, 134, 139, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {121, 125, 130, 135, 141, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {118, 122, 127, 132, 137, 143, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {115, 119, 124, 129, 133, 139, 144, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {112, 117, 121, 125, 130, 135, 140, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {110, 114, 118, 122, 127, 132, 137, 142, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {108, 112, 115, 120, 124, 128, 133, 138, 144, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {105, 109, 113, 117, 121, 125, 130, 135, 140, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {103, 107, 111, 114, 118, 123, 127, 131, 136, 141, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    {101, 105, 108, 112, 116, 120, 124, 128, 133, 138, 143, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 99, 103, 106, 110, 114, 117, 121, 125, 130, 134, 139, 144, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 98, 101, 104, 108, 111, 115, 119, 123, 127, 131, 136, 141, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 96,  99, 102, 106, 109, 113, 116, 120, 124, 128, 133, 137, 142, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 94,  97, 100, 104, 107, 110, 114, 118, 122, 126, 130, 134, 139, 144, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 92,  95,  99, 102, 105, 108, 112, 115, 119, 123, 127, 131, 135, 140, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 91,  94,  97, 100, 103, 106, 110, 113, 117, 120, 124, 128, 132, 137, 141, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 89,  92,  95,  98, 101, 104, 108, 111, 114, 118, 122, 126, 130, 134, 138, 143, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 88,  91,  94,  96,  99, 103, 106, 109, 112, 116, 119, 123, 127, 131, 135, 140, 144, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 86,  89,  92,  95,  98, 101, 104, 107, 110, 114, 117, 121, 124, 128, 132, 136, 141, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 85,  88,  91,  93,  96,  99, 102, 105, 108, 112, 115, 118, 122, 126, 129, 134, 138, 142, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 84,  86,  89,  92,  95,  97, 100, 103, 106, 110, 113, 116, 120, 123, 127, 131, 135, 139, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 83,  85,  88,  90,  93,  96,  99, 102, 105, 108, 111, 114, 117, 121, 124, 128, 132, 136, 140, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 81,  84,  86,  89,  92,  94,  97, 100, 103, 106, 109, 112, 115, 119, 122, 126, 129, 133, 137, 142, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 80,  83,  85,  88,  90,  93,  96,  98, 101, 104, 107, 110, 113, 116, 120, 123, 127, 131, 135, 139, 143, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 79,  81,  84,  86,  89,  91,  94,  97, 100, 102, 105, 108, 111, 114, 118, 121, 125, 128, 132, 136, 140, 144, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 78,  80,  83,  85,  87,  90,  93,  95,  98, 101, 104, 106, 109, 112, 116, 119, 122, 126, 129, 133, 137, 141, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 77,  79,  81,  84,  86,  89,  91,  94,  96,  99, 102, 105, 108, 111, 114, 117, 120, 123, 127, 130, 134, 138, 142, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 76,  78,  80,  83,  85,  87,  90,  92,  95,  98, 100, 103, 106, 109, 112, 115, 118, 121, 125, 128, 132, 135, 139, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 75,  77,  79,  81,  84,  86,  89,  91,  94,  96,  99, 101, 104, 107, 110, 113, 116, 119, 122, 126, 129, 133, 137, 141, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 74,  76,  78,  80,  83,  85,  87,  90,  92,  95,  97, 100, 103, 105, 108, 111, 114, 117, 120, 124, 127, 130, 134, 138, 142, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 73,  75,  77,  79,  81,  84,  86,  88,  91,  93,  96,  98, 101, 104, 107, 109, 112, 115, 118, 121, 125, 128, 132, 135, 139, 143, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 72,  74,  76,  78,  80,  83,  85,  87,  90,  92,  94,  97, 100, 102, 105, 108, 110, 113, 116, 119, 123, 126, 129, 133, 136, 140, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 71,  73,  75,  77,  79,  82,  84,  86,  88,  91,  93,  96,  98, 101, 103, 106, 109, 112, 114, 117, 120, 124, 127, 130, 134, 137, 141, 145, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 70,  72,  74,  76,  78,  80,  83,  85,  87,  89,  92,  94,  97,  99, 102, 104, 107, 110, 113, 116, 118, 122, 125, 128, 131, 135, 139, 142, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 69,  71,  73,  75,  77,  79,  82,  84,  86,  88,  91,  93,  95,  98, 100, 103, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 136, 140, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 68,  70,  72,  74,  76,  78,  80,  83,  85,  87,  89,  92,  94,  96,  99, 101, 104, 107, 109, 112, 115, 118, 121, 124, 127, 130, 134, 137, 141, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 67,  69,  71,  73,  75,  77,  79,  82,  84,  86,  88,  90,  93,  95,  98, 100, 102, 105, 108, 110, 113, 116, 119, 122, 125, 128, 131, 135, 138, 142, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 67,  68,  70,  72,  74,  76,  78,  81,  83,  85,  87,  89,  92,  94,  96,  99, 101, 104, 106, 109, 111, 114, 117, 120, 123, 126, 129, 132, 136, 139, 143, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 66,  68,  70,  72,  73,  75,  77,  80,  82,  84,  86,  88,  90,  93,  95,  97, 100, 102, 105, 107, 110, 112, 115, 118, 121, 124, 127, 130, 133, 137, 140, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 65,  67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  94,  96,  98, 101, 103, 106, 108, 111, 113, 116, 119, 122, 125, 128, 131, 134, 138, 141, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 64,  66,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  95,  97,  99, 102, 104, 107, 109, 112, 114, 117, 120, 123, 126, 129, 132, 135, 139, 143, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 63,  65,  67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  96,  98, 100, 103, 105, 108, 110, 113, 115, 118, 121, 124, 127, 130, 133, 136, 140, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 63,  65,  66,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  97,  99, 101, 104, 106, 109, 111, 114, 116, 119, 122, 125, 128, 131, 134, 138, 141, 145, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 62,  64,  66,  67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  98, 100, 102, 105, 107, 110, 112, 115, 117, 120, 123, 126, 129, 132, 135, 139, 142, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 61,  63,  65,  67,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  99, 101, 103, 106, 108, 111, 113, 116, 118, 121, 124, 127, 130, 133, 136, 140, 143, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 61,  62,  64,  66,  68,  70,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  97, 100, 102, 104, 107, 109, 112, 114, 117, 119, 122, 125, 128, 131, 134, 137, 141, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 60,  62,  63,  65,  67,  69,  71,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98, 101, 103, 105, 108, 110, 113, 115, 118, 120, 123, 126, 129, 132, 135, 138, 142, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 59,  61,  63,  65,  66,  68,  70,  72,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  97,  99, 102, 104, 106, 109, 111, 113, 116, 119, 121, 124, 127, 130, 133, 136, 139, 143, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 59,  60,  62,  64,  66,  67,  69,  71,  73,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98, 100, 103, 105, 107, 109, 112, 114, 117, 120, 122, 125, 128, 131, 134, 137, 140, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 58,  60,  61,  63,  65,  67,  68,  70,  72,  74,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  97,  99, 101, 103, 106, 108, 110, 113, 115, 118, 120, 123, 126, 129, 132, 135, 138, 141, 145, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 58,  59,  61,  62,  64,  66,  68,  69,  71,  73,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98, 100, 102, 104, 107, 109, 111, 114, 116, 119, 121, 124, 127, 130, 133, 136, 139, 142, 146, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 57,  59,  60,  62,  63,  65,  67,  68,  70,  72,  74,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  97,  99, 101, 103, 105, 108, 110, 112, 115, 117, 120, 122, 125, 128, 131, 134, 137, 140, 143, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 56,  58,  60,  61,  63,  64,  66,  68,  69,  71,  73,  75,  76,  78,  80,  82,  84,  86,  88,  89,  91,  93,  95,  98, 100, 102, 104, 106, 109, 111, 113, 116, 118, 121, 123, 126, 129, 132, 135, 138, 141, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 56,  57,  59,  61,  62,  64,  65,  67,  69,  70,  72,  74,  76,  77,  79,  81,  83,  85,  87,  88,  90,  92,  94,  96,  98, 101, 103, 105, 107, 109, 112, 114, 117, 119, 122, 124, 127, 130, 132, 135, 139, 142, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 55,  57,  58,  60,  62,  63,  65,  66,  68,  70,  71,  73,  75,  77,  78,  80,  82,  84,  86,  87,  89,  91,  93,  95,  97,  99, 101, 104, 106, 108, 110, 113, 115, 117, 120, 122, 125, 128, 131, 133, 136, 139, 143, 146, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 55,  56,  58,  59,  61,  63,  64,  66,  67,  69,  71,  72,  74,  76,  77,  79,  81,  83,  85,  86,  88,  90,  92,  94,  96,  98, 100, 102, 105, 107, 109, 111, 114, 116, 118, 121, 123, 126, 129, 131, 134, 137, 140, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 54,  56,  57,  59,  60,  62,  63,  65,  67,  68,  70,  72,  73,  75,  77,  78,  80,  82,  84,  86,  87,  89,  91,  93,  95,  97,  99, 101, 103, 105, 108, 110, 112, 114, 117, 119, 122, 124, 127, 130, 132, 135, 138, 141, 145, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 54,  55,  57,  58,  60,  61,  63,  64,  66,  68,  69,  71,  72,  74,  76,  78,  79,  81,  83,  85,  86,  88,  90,  92,  94,  96,  98, 100, 102, 104, 106, 108, 111, 113, 115, 118, 120, 123, 125, 128, 130, 133, 136, 139, 142, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 53,  55,  56,  58,  59,  61,  62,  64,  65,  67,  69,  70,  72,  73,  75,  77,  78,  80,  82,  84,  86,  87,  89,  91,  93,  95,  97,  99, 101, 103, 105, 107, 109, 112, 114, 116, 119, 121, 123, 126, 129, 131, 134, 137, 140, 143, 146, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 53,  54,  56,  57,  59,  60,  62,  63,  65,  66,  68,  69,  71,  73,  74,  76,  78,  79,  81,  83,  85,  86,  88,  90,  92,  94,  96,  98, 100, 102, 104, 106, 108, 110, 112, 115, 117, 119, 122, 124, 127, 129, 132, 135, 138, 141, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 52,  54,  55,  57,  58,  60,  61,  63,  64,  66,  67,  69,  70,  72,  74,  75,  77,  79,  80,  82,  84,  85,  87,  89,  91,  93,  95,  97,  99, 101, 103, 105, 107, 109, 111, 113, 116, 118, 120, 123, 125, 128, 130, 133, 136, 139, 142, 145, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 52,  53,  55,  56,  57,  59,  60,  62,  63,  65,  67,  68,  70,  71,  73,  74,  76,  78,  79,  81,  83,  85,  86,  88,  90,  92,  94,  96,  97,  99, 101, 103, 105, 108, 110, 112, 114, 116, 119, 121, 124, 126, 129, 131, 134, 137, 140, 143, 146, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 51,  53,  54,  56,  57,  58,  60,  61,  63,  64,  66,  67,  69,  71,  72,  74,  75,  77,  79,  80,  82,  84,  85,  87,  89,  91,  93,  95,  96,  98, 100, 102, 104, 106, 108, 111, 113, 115, 117, 120, 122, 124, 127, 129, 132, 135, 138, 141, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 51,  52,  54,  55,  56,  58,  59,  61,  62,  64,  65,  67,  68,  70,  71,  73,  75,  76,  78,  79,  81,  83,  85,  86,  88,  90,  92,  93,  95,  97,  99, 101, 103, 105, 107, 109, 111, 114, 116, 118, 120, 123, 125, 128, 130, 133, 136, 138, 141, 144, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 50,  52,  53,  55,  56,  57,  59,  60,  62,  63,  65,  66,  68,  69,  71,  72,  74,  75,  77,  79,  80,  82,  84,  85,  87,  89,  91,  92,  94,  96,  98, 100, 102, 104, 106, 108, 110, 112, 114, 117, 119, 121, 124, 126, 129, 131, 134, 137, 139, 142, 145, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 50,  51,  53,  54,  55,  57,  58,  60,  61,  63,  64,  66,  67,  69,  70,  72,  73,  75,  76,  78,  80,  81,  83,  85,  86,  88,  90,  92,  93,  95,  97,  99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 120, 122, 124, 127, 129, 132, 135, 137, 140, 143, 146, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 49,  51,  52,  54,  55,  56,  58,  59,  61,  62,  63,  65,  66,  68,  69,  71,  72,  74,  76,  77,  79,  80,  82,  84,  85,  87,  89,  91,  92,  94,  96,  98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 121, 123, 125, 128, 130, 133, 135, 138, 141, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 49,  50,  52,  53,  54,  56,  57,  59,  60,  61,  63,  64,  66,  67,  69,  70,  72,  73,  75,  76,  78,  80,  81,  83,  85,  86,  88,  90,  91,  93,  95,  97,  99, 101, 102, 104, 106, 108, 110, 113, 115, 117, 119, 121, 124, 126, 129, 131, 134, 136, 139, 142, 145, 148, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 49,  50,  51,  53,  54,  55,  57,  58,  59,  61,  62,  64,  65,  67,  68,  70,  71,  73,  74,  76,  77,  79,  80,  82,  84,  85,  87,  89,  90,  92,  94,  96,  98,  99, 101, 103, 105, 107, 109, 111, 113, 115, 118, 120, 122, 124, 127, 129, 132, 134, 137, 140, 143, 146, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 48,  50,  51,  52,  53,  55,  56,  58,  59,  60,  62,  63,  65,  66,  67,  69,  70,  72,  73,  75,  77,  78,  80,  81,  83,  85,  86,  88,  90,  91,  93,  95,  97,  98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 121, 123, 125, 128, 130, 133, 135, 138, 141, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150, 150},
    { 48,  49,  50,  52,  53,  54,  56,  57,  58,  60,  61,  63,  64,  65,  67,  68,  70,  71,  73,  74,  76,  77,  79,  80,  82,  84,  85,  87,  89,  90,  92,  94,  96,  97,  99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 124, 126, 128, 131, 133, 136, 139, 142, 144, 147, 150, 150, 150, 150, 150, 150, 150, 150},
    { 47,  49,  50,  51,  53,  54,  55,  57,  58,  59,  61,  62,  63,  65,  66,  68,  69,  71,  72,  74,  75,  77,  78,  80,  81,  83,  84,  86,  88,  89,  91,  93,  95,  96,  98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 125, 127, 129, 132, 134, 137, 140, 142, 145, 148, 150, 150, 150, 150, 150, 150, 150},
    { 47,  48,  50,  51,  52,  53,  55,  56,  57,  59,  60,  61,  63,  64,  66,  67,  69,  70,  71,  73,  74,  76,  77,  79,  81,  82,  84,  85,  87,  89,  90,  92,  94,  95,  97,  99, 101, 103, 105, 106, 108, 110, 112, 114, 116, 119, 121, 123, 125, 128, 130, 133, 135, 138, 140, 143, 146, 149, 150, 150, 150, 150, 150, 150},
    { 47,  48,  49,  50,  52,  53,  54,  56,  57,  58,  60,  61,  62,  64,  65,  66,  68,  69,  71,  72,  74,  75,  77,  78,  80,  81,  83,  84,  86,  88,  89,  91,  93,  94,  96,  98, 100, 102, 103, 105, 107, 109, 111, 113, 115, 117, 119, 122, 124, 126, 128, 131, 133, 136, 138, 141, 144, 147, 150, 150, 150, 150, 150, 150},
    { 46,  47,  49,  50,  51,  53,  54,  55,  56,  58,  59,  60,  62,  63,  65,  66,  67,  69,  70,  72,  73,  75,  76,  78,  79,  81,  82,  84,  85,  87,  89,  90,  92,  94,  95,  97,  99, 101, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 125, 127, 129, 132, 134, 137, 139, 142, 145, 148, 150, 150, 150, 150, 150},
    { 46,  47,  48,  50,  51,  52,  53,  55,  56,  57,  59,  60,  61,  63,  64,  65,  67,  68,  70,  71,  72,  74,  75,  77,  78,  80,  81,  83,  84,  86,  88,  89,  91,  93,  94,  96,  98,  99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 128, 130, 132, 135, 137, 140, 143, 146, 149, 150, 150, 150, 150},
    { 45,  47,  48,  49,  50,  52,  53,  54,  55,  57,  58,  59,  61,  62,  63,  65,  66,  68,  69,  70,  72,  73,  75,  76,  78,  79,  81,  82,  84,  85,  87,  88,  90,  92,  93,  95,  97,  98, 100, 102, 104, 106, 108, 109, 111, 113, 115, 117, 120, 122, 124, 126, 128, 131, 133, 136, 138, 141, 144, 146, 149, 150, 150, 150},
};