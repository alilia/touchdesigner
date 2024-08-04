#ifndef TACHINPUT_H
#define TACHINPUT_H

#include <TDComm.h>

class TachInput : public TDComm {
public:
	TachInput(int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom);
	void begin() override;
	void receiveSerialData(byte incomingByte) override;
	void countRPM();
	void loop();

private:
	// this pointer magic was done by chatgpt
	static void isrCountRPM();   // Static member function for ISR
	static TachInput* instance;  // Static instance pointer

	int baseSpeed;
	volatile int RPMcounter;
	float adjuster;
	unsigned long lastMillis;
};

#endif // TACHINPUT_H
