#include "RGBLed.h"
#include "RGBLedController.h"

// Create an instance of RGBLedController
RGBLedController controller;

// Create instances of RGBLed
RGBLed matrix1(controller, 0);
RGBLed matrix2(controller, 1);

void setup() {
	Serial.begin(115200);

	// Set data pin for the controller
	controller.setDataPin(6);

	// Set up the first matrix
	matrix1.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFF);

	matrix1.setPixelFormat(RGBLed::PIXEL_FORMAT_RGB);
	matrix1.setColorDepth(6);
	// matrix1.setFlipAxis(RGBLed::FLIP_AXIS_X); // leaving here, to make this feature visible
	// matrix1.setInputScale(2); // leaving here, to make this feature visible

	matrix1.setResolution(8, 8);
	matrix1.begin();

	// Set up the second matrix
	matrix2.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFE);

	matrix2.setPixelFormat(RGBLed::PIXEL_FORMAT_RGB);
	matrix2.setColorDepth(6);
	// matrix2.setFlipAxis(RGBLed::FLIP_AXIS_X); // leaving here, to make this feature visible
	// matrix2.setInputScale(2); // leaving here, to make this feature visible

	matrix2.setResolution(8, 32);
	matrix2.begin();

	// Begin the controller
	controller.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();
		matrix1.receiveSerialData(incomingByte);
		matrix2.receiveSerialData(incomingByte);
	}
}
