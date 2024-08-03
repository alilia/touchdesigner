#include "TachInput.h"

TachInput* TachInput::instance = nullptr;

// this pointer magic was done by chatgpt
void TachInput::isrCountRPM() {
	if (instance) {
		instance->countRPM();
	}
}

TachInput::TachInput() {
	state = WAITING_FOR_DATA;
	adjuster = 0.1; // how often to eval tach counter
	baseSpeed = 22000; // rmp to chop
	lastMillis = 0;
}

void TachInput::begin() {
	begin(0xFE, 0xFC); // default RMP (output) marker and default base speed (input) marker
}

void TachInput::begin(int RPMMarkerCustom, int baseSpeedMarkerCustom) {
	RPMMarker = RPMMarkerCustom;
	baseSpeedMarker = baseSpeedMarkerCustom;

	pinMode(TACH_PIN, INPUT_PULLUP);
	pinMode(BASE_SPEED_PIN, OUTPUT);

	instance = this;
	attachInterrupt(digitalPinToInterrupt(TACH_PIN), TachInput::isrCountRPM, FALLING);

	analogWrite(BASE_SPEED_PIN, 255);
}

void TachInput::countRPM() {
	RPMcounter++;
}

void TachInput::receiveSerialData() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();
		processIncomingByte(incomingByte);
	}
}

void TachInput::processIncomingByte(byte incomingByte) {
	if (state == WAITING_FOR_DATA) {
		if (incomingByte == baseSpeedMarker) {
			state = RECEIVING_DATA;
		}
	} else if (state == RECEIVING_DATA) {
		baseSpeed = incomingByte * 1000;
		state = WAITING_FOR_DATA;
	}
}

void TachInput::publishRPM(int RPM) {
	Serial.println(RPMMarker);
	Serial.println(RPM);
}

void TachInput::tachLoop() {
	if (millis() - lastMillis >= (1000 * adjuster)) {
		detachInterrupt(digitalPinToInterrupt(TACH_PIN));
		int RPM = (RPMcounter * (60 / 2)) / adjuster - baseSpeed;

		if (RPM > 0) {
			publishRPM(RPM);
		}
		RPMcounter = 0;

		lastMillis = millis();
		attachInterrupt(digitalPinToInterrupt(TACH_PIN), TachInput::isrCountRPM, FALLING);
	}
}
