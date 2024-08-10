#ifndef RGBLED_H
#define RGBLED_H

#include <TDComm.h>
#include <FastLED.h>

// "dynamic" data pin hack (digital pins for arduino r4 wifi)
#define DATA_PIN_00 0
#define DATA_PIN_01 1
#define DATA_PIN_02 2
#define DATA_PIN_03 3
#define DATA_PIN_04 4
#define DATA_PIN_05 5
#define DATA_PIN_06 6
#define DATA_PIN_07 7
#define DATA_PIN_08 8
#define DATA_PIN_09 9
#define DATA_PIN_10 10
#define DATA_PIN_11 11
#define DATA_PIN_12 12
#define DATA_PIN_13 13

class RGBLed : public TDComm {
public:
	RGBLed(int matrixWidthCustom, int matrixHeightCustom, int inputMarkerCustom, int outputDataPinCustom);
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
