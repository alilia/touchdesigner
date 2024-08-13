#ifndef TDCOMM_H
#define TDCOMM_H

#include <Arduino.h>

class TDComm {
public:
	TDComm();
	TDComm(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom, int timeoutMiliSecCustom);
	virtual void begin(); // method triggered in Arduino's setup()
	virtual void receiveSerialData(byte incomingByte) = 0; // method for processing serial data (usually in processIncomingByte())
	virtual void loop(); // method for processing input data pin

protected:
	int inputMarker; // will look for this byte to start porcessing serial data
	int outputMarker; // will output this byte before outputting data on serial

	int inputDataPin; // will look for incoming signal on this Arduino pin
	int outputDataPin; // will output signal on this Arduino pin

	String outputSerialData; // will output this to serial
	int outputPinData; // will output this signal on pin

	int timeoutMiliSec;
	unsigned long lastMiliSec;

	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	virtual void writeDataToSerial();
	virtual void writeDataToPin();

	void checkStateTimeout();
};

#endif // TDCOMM_H
