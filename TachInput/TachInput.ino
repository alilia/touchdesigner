#include "TachInput.h"

TachInput tach(2, 0xFC, 9, 0xFE);

void setup() {
	Serial.begin(9600);
	tach.begin();
}

void loop() {
	while (Serial.available() > 0) {
		byte incomingByte = Serial.read();

		tach.receiveSerialData(incomingByte);
	}

	tach.loop();
}
