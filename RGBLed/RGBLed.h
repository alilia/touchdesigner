#ifndef RGBLED_H
#define RGBLED_H

#include <TDComm.h>
#include <FastLED.h>

class RGBLed : public TDComm {
public:
	RGBLed(int matrixWidthCustom, int matrixHeightCustom, int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom);
	~RGBLed();
	void begin() override;
	void receiveSerialData(byte incomingByte) override;
	void processIncomingByte(byte incomingByte);

	void setPixel(int x, int y, CRGB color);
	void setBrightness(uint8_t brightness);
	void update();
	void loop();

private:
	int matrixWidth;
	int matrixHeight;
	int numLeds;
	CRGB* leds; // pointer magic provided by chatgpt
	byte* buffer; // pointer magic provided by chatgpt
	int currentRow;
	int bytesReceived;
};

#endif // RGBLED_H
