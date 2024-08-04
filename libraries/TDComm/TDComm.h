#ifndef TDCOMM_H
#define TDCOMM_H

#include <Arduino.h>

class TDComm {
public:
	TDComm();
	TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom);
	virtual void begin();
	virtual void receiveSerialData(byte incomingByte) = 0;
	virtual void loop() = 0;

protected:
	int inputMarker;
	int outputMarker;

	int inputDataPin;
	int outputDataPin;

	String outputSerialData;
	int outputPinData;

	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	virtual void writeDataToSerial();
	virtual void writeDataToPin();
};

#endif // TDCOMM_H
