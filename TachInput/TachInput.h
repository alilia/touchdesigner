#ifndef TACHINPUT_H
#define TACHINOUT_H

#include <Arduino.h>

#define TACH_PIN 2
#define BASE_SPEED_PIN 9

class TachInput {
public:
	TachInput();
	void begin();
	void begin(int RPMMarkerCustom, int baseSpeedMarkerCustom);
	void receiveSerialData();
	void tachLoop();
	void countRPM();

private:
	// this pointer magic was done by chatgpt
	static void isrCountRPM();   // Static member function for ISR
	static TachInput* instance;  // Static instance pointer

	void publishRPM(int RPM);
	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	int baseSpeed;
	void processIncomingByte(byte incomingByte);
	int RPMMarker;
	int baseSpeedMarker;
	volatile int RPMcounter;
	unsigned long lastMillis;
	float adjuster;
};

#endif  // TACHINPUT_H