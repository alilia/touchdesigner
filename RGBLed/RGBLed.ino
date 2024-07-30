#include "RGBLed.h"

RGBLed matrix;

void setup() {
  Serial.begin(115200);

  matrix.begin();
  // matrix.begin(0xFF);  // The first parameter - new frame marker (default: 0xFF).
  // The default data pin is 6. Can be updated in header (DATA_PIN constant).
}

void loop() {
  matrix.receiveSerialData();
}
