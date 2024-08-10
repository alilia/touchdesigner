#include "TDComm.h"

TDComm::TDComm() { /* nothing to do here */ }

TDComm::TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom) {
	/*
		inputDataPinCustom  -   -1 if n/a
		inputMarkerCustom   -   will wait for that byte on serial, before proceeding to process the data
		outputDataPinCustom -   -1 if n/a
		outputMarkerCustom  -   will output this byte first before the actual data
	*/

	inputDataPin = inputDataPinCustom;
	outputDataPin = outputDataPinCustom;

	if (inputMarkerCustom > -1) {
		inputMarker = inputMarkerCustom;
	}

	if (outputMarkerCustom > -1) {
		outputMarker = outputMarkerCustom;
	}

	state = WAITING_FOR_DATA;
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

void TDComm::loop() {}

/*
void TDComm::receiveSerialData(byte incomingByte) {
	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
		}
	} else if (state == RECEIVING_DATA) {

		//
		// put your processing here
		//

		state = WAITING_FOR_DATA;
	}
}
*/
