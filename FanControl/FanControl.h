#ifndef FANCONTROL_H
#define FANCONTROL_H

#include <TDComm.h>

class FanControl : public TDComm {
public:
	FanControl();
	void begin() override;
	void receiveSerialData(byte incomingByte) override;
};

#endif // FANCONTROL_H
