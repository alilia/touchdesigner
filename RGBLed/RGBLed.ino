#include "RGBLed.h"

RGBLed matrix;

void setup() {
	Serial.begin(115200);

	matrix.setInputCommunication(1, 0xFF);
	matrix.setOutputCommunication(0, 6);

	matrix.setResolution(8, 8);
	// matrix.setPixelFormat(0);
	matrix.setColorDepth(6);

	matrix.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		matrix.receiveSerialData(incomingByte);
	}
}
