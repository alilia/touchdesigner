#include "FanControl.h"

FanControl::FanControl() { /* nothing to do here*/ }

void FanControl::begin() {
	TDComm::begin();

	outputPinData = 0;
	writeDataToPin();
}

void FanControl::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	switch(state) {
		case WAITING_FOR_MARKER:
			if (incomingByte == inputMarker) {
				state = RECEIVING_DATA;
				lastMiliSec = millis();
			}

			break;

		case RECEIVING_DATA:
			outputPinData = incomingByte;
			writeDataToPin();
			resetState();

			break;
	}
}
