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
	RGBLed();
	~RGBLed();

	enum PixelFormat { PIXEL_FORMAT_RGB, PIXEL_FORMAT_MONO };
	enum FlipAxisTypes { FLIP_AXIS_NONE, FLIP_AXIS_X, FLIP_AXIS_Y } ;

	void setResolution(int matrixWidthCustom, int matrixHeightCustom);
	void setPixelFormat(PixelFormat pixelFormatCustom);
	void setColorDepth(int colorDepthCustom);

	void setInputScale(int inputScaleCustom);
	void setFlipAxis(FlipAxisTypes flipAxisCustom);

	int calculateLedIndex(int x, int y);

	void begin() override;
	void receiveSerialData(byte incomingByte) override;
	void processIncomingByteFrame(byte incomingByte);
	void processIncomingByteLookup(byte incomingByte);


private:
	void unpack_values(byte* buffer, int* values, int length, int bitsPerValue);

	int matrixWidth;
	int matrixHeight;
	int matrixWidthScaled;
	int matrixHeightScaled;

	int numLeds;
	CRGB* leds; // pointer magic provided by chatgpt
	byte* buffer; // pointer magic provided by chatgpt

	enum ReceivingDataType { RECEIVING_NONE, RECEIVING_FRAME, RECEIVING_LOOKUP } receivingDataType;
	virtual void resetState() override;

	void setPixel(int x, int y, CRGB color);
	void update();

	int inputScale;
	int colorDepth;
	FlipAxisTypes flipAxis;

	PixelFormat pixelFormat;
	int pixelFormatMultiplier;

	CRGB lookupTable[256];

	int bytesReceived;
};

#endif // RGBLED_H
