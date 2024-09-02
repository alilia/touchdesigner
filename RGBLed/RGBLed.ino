#include "RGBLed.h"

RGBLed matrix;

void setup() {
	Serial.begin(115200);

	matrix.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFF);
	matrix.setOutputCommunication(TDComm::TDCOMM_PIN, 6);

	matrix.setPixelFormat(RGBLed::PIXEL_FORMAT_MONO);
	matrix.setColorDepth(6);
	matrix.setResolution(8, 8);

	matrix.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		matrix.receiveSerialData(incomingByte);
	}
}
