#include "TachInput.h"

TachInput* TachInput::instance = nullptr;

// this pointer magic was done by chatgpt
void TachInput::isrCountRPM() {
	if (instance) {
		instance->countRPM();
	}
}

TachInput::TachInput(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom)
	: TDComm(inputDataPinCustom, inputMarkerCustom, outputDataPinCustom, outputMarkerCustom, 1000),
		adjuster(0.1),
		baseSpeed(22000),
		lastMillis(0),
		RPMcounter(0)
	{}

void TachInput::begin() {
	TDComm::begin(); // Ensure base class initialization
	instance = this;
	attachInterrupt(digitalPinToInterrupt(inputDataPin), TachInput::isrCountRPM, FALLING);

	outputPinData = 255;
	writeDataToPin();
}

void TachInput::countRPM() {
	RPMcounter++;
}

void TachInput::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
		}
	} else if (state == RECEIVING_DATA) {
		baseSpeed = incomingByte * 1000;
		state = WAITING_FOR_DATA;
	}
}

void TachInput::loop() {
	if (millis() - lastMillis >= (1000 * adjuster)) {
		detachInterrupt(digitalPinToInterrupt(inputDataPin));
		int RPM = (RPMcounter * (60 / 2)) / adjuster - baseSpeed;

		if (RPM > 0) {
			outputSerialData = String(RPM);
			writeDataToSerial();
		}
		RPMcounter = 0;

		lastMillis = millis();
		attachInterrupt(digitalPinToInterrupt(inputDataPin), TachInput::isrCountRPM, FALLING);
	}
}
