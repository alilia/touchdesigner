#ifndef FANCONTROL_H
#define FANCONTROL_H

#include <Arduino.h>

#define DATA_PIN 9

class FanControl {
public:
	FanControl();
	void begin();
	void begin(int fanSpeedMarkerCustom);
	void publishFanSpeed();
	void receiveSerialData();

private:
	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	int fanSpeed;
	void processIncomingByte(byte incomingByte);
	int fanSpeedMarker;
};

#endif
