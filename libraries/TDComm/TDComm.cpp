#include "TDComm.h"

TDComm::TDComm() { /* nothing to do here */ }

TDComm::TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom, int timeoutMiliSecCustom)
	: inputDataPin(inputDataPinCustom),
	outputDataPin(outputDataPinCustom),
	timeoutMiliSec(timeoutMiliSecCustom)
	{
	/*
		inputDataPinCustom  -   -1 if n/a
		inputMarkerCustom   -   will wait for that byte on serial, before proceeding to process the data
		outputDataPinCustom -   -1 if n/a
		outputMarkerCustom  -   will output this byte first before the actual data
	*/

	if (inputMarkerCustom > -1) {
		inputMarker = inputMarkerCustom;
	}

	if (outputMarkerCustom > -1) {
		outputMarker = outputMarkerCustom;
	}

	resetState();
}

void TDComm::begin() {
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

		resetState()
	}
}
*/
