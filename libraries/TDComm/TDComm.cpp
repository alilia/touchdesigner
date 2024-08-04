#include "TDComm.h"

void TDComm::TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom) {
	/*
		inptuDataPinCustom	-	-1 if n/a
		inputMarkerCustom	-	will wait for that byte on serial, before proceeding to process the data
		outptuDataPinCustom	-	-1 if n/a
		outputMarkerCustom	-	will output this byte first before the actual data
	*/

	inputDataPin = inputDataPinCustom;
	outputDataPin = outputDataPinCustom;

	if (inputMarkerCustom) {
		inputMarker = inputMarkerCustom;
	}

	if (outputMarkerCustom) {
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

void TDComm::loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();
		receiveSerialData(incomingByte);
	}
}

void TDComm::writeDataToSerial() {
	Serial.println(outputMarker);
	Serial.println(outputSerialData);
}

void TDComm::writeDataToPin() {
	analogWrite(outptuDataPin, outputPinData);
}

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
