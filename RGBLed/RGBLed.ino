#include "RGBLed.h"

RGBLed matrix(16, 16, -1, 0xFF, 6, -1);

void setup() {
	Serial.begin(115200);

	matrix.begin();
	// matrix.begin(0xFF);  // The first parameter - new frame marker (default: 0xFF).
	// The default data pin is 6. Can be updated in header (DATA_PIN constant).
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		matrix.receiveSerialData(incomingByte);
	}
}
