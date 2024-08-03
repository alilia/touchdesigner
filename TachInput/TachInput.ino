#include "TachInput.h"

TachInput tach;

void setup() {
	Serial.begin(9600);
	tach.begin();
}

void loop() {
	tach.receiveSerialData();
	tach.tachLoop();
}
