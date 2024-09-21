#include "RGBLed.h"
#include "RGBLedController.h"

RGBLedController controller;

RGBLed matrix1(controller);
RGBLed matrix2(controller);
RGBLed matrix3(controller);

void setup() {
	Serial.begin(115200);

	controller.setDataPin(6);

	matrix1.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFF);

	matrix1.setPixelFormat(RGBLed::PIXEL_FORMAT_MONO);
	matrix1.setColorDepth(6);
	// matrix1.setFlipAxis(RGBLed::FLIP_AXIS_X); // leaving here, to make this feature visible
	// matrix1.setInputScale(2); // leaving here, to make this feature visible

	matrix1.setResolution(8, 8);
	matrix1.begin();

	matrix2.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFE);

	matrix2.setPixelFormat(RGBLed::PIXEL_FORMAT_MONO);
	matrix2.setColorDepth(6);
	// matrix2.setFlipAxis(RGBLed::FLIP_AXIS_X); // leaving here, to make this feature visible
	// matrix2.setInputScale(2); // leaving here, to make this feature visible

	matrix2.setResolution(8, 32);
	matrix2.begin();

	matrix3.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFD);

	matrix3.setPixelFormat(RGBLed::PIXEL_FORMAT_MONO);
	matrix3.setColorDepth(6);
	// matrix3.setFlipAxis(RGBLed::FLIP_AXIS_X); // leaving here, to make this feature visible
	// matrix3.setInputScale(2); // leaving here, to make this feature visible

	matrix3.setResolution(8, 8);
	matrix3.begin();

	controller.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		matrix1.receiveSerialData(incomingByte);
		matrix2.receiveSerialData(incomingByte);
		matrix3.receiveSerialData(incomingByte);
	}
}
