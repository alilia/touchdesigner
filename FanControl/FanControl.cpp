#include "FanControl.h"

FanControl::FanControl(int inputMarkerCustom, int outputDataPinCustom)
	: TDComm(-1, inputMarkerCustom, outputDataPinCustom, -1)
	{}

void FanControl::begin() {
	TDComm::begin();

	outputPinData = 0;
	writeDataToPin();
}

void FanControl::receiveSerialData(byte incomingByte) {
	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
		}
	} else if (state == RECEIVING_DATA) {
		outputPinData = incomingByte;
		writeDataToPin();
		state = WAITING_FOR_DATA;
	}
}

void FanControl::loop() {}
