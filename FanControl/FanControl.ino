#include "FanControl.h"

FanControl fan;

void setup() {
	Serial.begin(9600);

	fan.setInputCommunication(1, 0xFD);
	fan.setOutputCommunication(0, 9);
	fan.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		fan.receiveSerialData(incomingByte);
	}
}
