#include "FanControl.h"

FanControl::FanControl() { /* nothing to do here*/ }

void FanControl::begin() {
	TDComm::begin();

	outputPinData = 0;
	writeDataToPin();
}

void FanControl::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
			lastMiliSec = millis();
		}
	} else if (state == RECEIVING_DATA) {
		outputPinData = incomingByte;
		writeDataToPin();
		resetState();
	}
}
