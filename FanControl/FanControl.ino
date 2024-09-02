#include "FanControl.h"

FanControl fan;

void setup() {
	Serial.begin(9600);

	fan.setInputCommunication(TDComm::TDCOMM_SERIAL, 0xFD);
	fan.setOutputCommunication(TDComm::TDCOMM_PIN, 9);
	fan.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		fan.receiveSerialData(incomingByte);
	}
}
