#include "FanControl.h"

FanControl fan(0xFD, 9);

void setup() {
	Serial.begin(9600);
	fan.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		fan.receiveSerialData(incomingByte);
	}
}
