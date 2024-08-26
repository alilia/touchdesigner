#include "RGBLed.h"

RGBLed matrix(8, 8, 6, 0xFF, 6);

void setup() {
	Serial.begin(115200);

	matrix.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		matrix.receiveSerialData(incomingByte);
	}
}
