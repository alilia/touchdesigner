#include "FanControl.h"

FanControl fan;

void setup() {
	Serial.begin(9600);

	fan.begin();
	// fan.begin(0xFD);  // The first parameter - new fan speed marker (default: 0xFD).
	// The default data pin is 9. Can be updated in header (DATA_PIN constant). Make sure, it is a PWM-enabled.
}

void loop() {
	fan.receiveSerialData();
}
