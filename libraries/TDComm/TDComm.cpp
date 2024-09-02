#include "TDComm.h"

TDComm::TDComm()
	:
		inputDataPin(-1),
		inputMarker(-999),
		outputDataPin(-1),
		outputMarker(-999),
		timeoutMiliSec(1000)
	{ /* nothing to do here */ }

void TDComm::setInputCommunication(TypeOfComm typeOfComm, int idOfComm) {
	switch(typeOfComm) {
		case TDCOMM_PIN:
			inputDataPin = idOfComm; // pin number
			break;

		case TDCOMM_SERIAL:
			inputMarker = idOfComm; // start of data marker
			break;
	}
}

void TDComm::setOutputCommunication(TypeOfComm typeOfComm, int idOfComm) {
	/*
		## idOfComm
		* In case of pin : pin nr
		* In case of serial : marker (int)
	*/

	switch(typeOfComm) {
		case TDCOMM_PIN:
			outputDataPin = idOfComm; // pin number
			break;

		case TDCOMM_SERIAL:
			outputMarker = idOfComm; // start of data marker
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
