#include "TDComm.h"

/*

I would imagine the usage as follows:

RGBLed matrix();

void setup() {
	...
	matrix.setInputCommunicationType(0, 0xFF); // first arg - pin or serial, second - either pin nr or marker
	matrix.setOutputCommunication(1, 6); // first arg - pin or serial, second - either pin nr or marker
	matrix.setTimeout(1000);

	matrix.setResolution(8, 8);
	matrix.setPixelFormat(0); // 0 - rgb, 1 - monochrome
	matrix.setColorDepth(6); // bitpacking stuff
	matrix.setLookup(); // dunno yet how

	matrix.begin();
}

void loop() {
	...
	matrix.receiveSerialData(incomingByte);
}


*/

TDComm::TDComm()
	:
		inputDataPin(-1),
		inputMarker(-999),
		outputDataPin(-1),
		outputMarker(-999),
		timeoutMiliSec(1000)
	{ /* nothing to do here */ }

void TDComm::setInputCommunication(int typeOfComm, int idOfComm) {
	/*
		## typeOfComm
		* `0` : via pin
		* `1` : via serial

		## idOfComm
		* In case of pin : pin nr
		* In case of serial : marker (int)
	*/

	switch(typeOfComm) {
		case 0:
			inputDataPin = idOfComm;
			break;

		case 1:
			inputMarker = idOfComm;
			break;
	}
}

void TDComm::setOutputCommunication(int typeOfComm, int idOfComm) {
	/*
		## typeOfComm
		* `0` : via pin
		* `1` : via serial

		## idOfComm
		* In case of pin : pin nr
		* In case of serial : marker (int)
	*/

	switch(typeOfComm) {
		case 0:
			outputDataPin = idOfComm;
			break;

		case 1:
			outputMarker = idOfComm;
			break;
	}
}

void TDComm::setTimeout(int timeoutMiliSecCustom) {
	timeoutMiliSec = timeoutMiliSecCustom;
}

void TDComm::begin() {
	resetState();

	if (inputDataPin > -1) {
		pinMode(inputDataPin, INPUT);
	}

	if (outputDataPin > -1) {
		pinMode(outputDataPin, OUTPUT);
	}
}

void TDComm::writeDataToSerial() {
	if (outputMarker > -1) {
		Serial.println(outputMarker);
		Serial.println(outputSerialData);
	}
}

void TDComm::writeDataToPin() {
	if (outputDataPin > -1 && outputPinData >= 0 && outputPinData < 256) {
		analogWrite(outputDataPin, outputPinData);
	}
}

void TDComm::checkStateTimeout() {
	if (state != WAITING_FOR_DATA) {
		if (millis() - lastMiliSec > timeoutMiliSec) {
			resetState();
		}
	}
}

void TDComm::resetState() {
	state = WAITING_FOR_DATA;
	lastMiliSec = -1;
}

void TDComm::loop() {}

/*
void TDComm::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
			lastMiliSec = millis();
		}
	} else if (state == RECEIVING_DATA) {

		//
		// put your processing here
		//

		resetState();
	}
}
*/
