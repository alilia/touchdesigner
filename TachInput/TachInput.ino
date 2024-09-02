#include "TachInput.h"

TachInput tach;

void setup() {
	Serial.begin(9600);

	tach.setInputCommunication(TDComm::TDCOMM_PIN, 2); // reading value on arduino from fan pin
	tach.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFC); // reading (base speed) value on arduino from serial
	tach.setOutputCommunication(TDComm::TDCOMM_PIN, 9); // fan base speed pin (why don't I put it to 5v?)
	tach.setOutputCommunication(TDComm::TDCOMM_SERIAL, 0xFE); // publishing fan rpm value to serial

	tach.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		tach.receiveSerialData(incomingByte);
	}

	tach.loop();
}
