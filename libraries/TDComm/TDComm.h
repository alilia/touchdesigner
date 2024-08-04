#ifndef TDCOMM_H
#define TDCOMM_H

#include <Arduino.h>

class TDComm {
public:
	TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom);
	virtual void begin();
	void loop();

protected:
	int inputMarker;
	int outputMarker;

	int inputDataPin;
	int outputDataPin;

	char outputSerialData;
	int outputPinData;

	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	virtual void receiveSerialData(byte incomingByte);
	virtual void writeDataToSerial();
	virtual void writeDataToPin();
};

#endif // TDCOMM_H
