#include "TachInput.h"

TachInput* TachInput::instance = nullptr;

// this pointer magic was done by chatgpt
void TachInput::isrCountRPM() {
	if (instance) {
		instance->countRPM();
	}
}

TachInput::TachInput()
	:
		adjuster(0.1), // how often to read fan rpm value
		lastMillis(0),
		RPMcounter(0)
	{
		setBaseSpeed(22000);
	}

void TachInput::begin() {
	TDComm::begin();

	instance = this;
	attachInterrupt(digitalPinToInterrupt(inputDataPin), TachInput::isrCountRPM, FALLING);

	outputPinData = 255; // output all 5V to give some base speed to a 12V coolig fan
	writeDataToPin();
}

void TachInput::countRPM() {
	RPMcounter++;
}

void TachInput::setBaseSpeed(int baseSpeedCustom) {
	baseSpeed = baseSpeedCustom * 1000;
}

void TachInput::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	switch(state) {
		case WAITING_FOR_MARKER:
			if (incomingByte == inputMarker) {
				state = RECEIVING_DATA;
				lastMiliSec = millis();
			}

			break;

		case RECEIVING_DATA:
			setBaseSpeed(incomingByte);
			resetState();

			break;
	}
}

void TachInput::loop() {
	if (millis() - lastMillis >= (1000 * adjuster)) {
		detachInterrupt(digitalPinToInterrupt(inputDataPin));
		int RPM = (RPMcounter * (60 / 2)) / adjuster - baseSpeed; // two interruption per full rotation

		if (RPM > 0) { // publishes rpm only if higher than set base speed
			outputSerialData = String(RPM);
			writeDataToSerial();
		}
		RPMcounter = 0;

		lastMillis = millis();
		attachInterrupt(digitalPinToInterrupt(inputDataPin), TachInput::isrCountRPM, FALLING);
	}
}
