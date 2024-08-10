#ifndef FANCONTROL_H
#define FANCONTROL_H

#include <TDComm.h>

class FanControl : public TDComm {
public:
	FanControl(int inputMarkerCustom, int outputDataPinCustom);
	void begin() override;
	void receiveSerialData(byte incomingByte) override;
	void loop();
};

#endif // FANCONTROL_H
