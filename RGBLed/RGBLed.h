#ifndef RGBLED_H
#define RGBLED_H

#include <TDComm.h>
#include <FastLED.h>
#include "RGBLedController.h"

class RGBLed : public TDComm {
public:
	RGBLed(RGBLedController& controller);
	~RGBLed();

	enum PixelFormat { PIXEL_FORMAT_RGB, PIXEL_FORMAT_MONO };
	enum FlipAxisTypes { FLIP_AXIS_NONE, FLIP_AXIS_X, FLIP_AXIS_Y };

	void setResolution(int matrixWidthCustom, int matrixHeightCustom);
	void setPixelFormat(PixelFormat pixelFormatCustom);
	void setColorDepth(int colorDepthCustom);

	void setInputScale(int inputScaleCustom);
	void setFlipAxis(FlipAxisTypes flipAxisCustom);

	void begin() override;
	void receiveSerialData(byte incomingByte) override;

	int getNumLeds() const;

private:
	void processIncomingByteFrame(byte incomingByte);
	void processIncomingByteLookup(byte incomingByte);
	void unpack_values(byte* buffer, int* values, int length, int bitsPerValue);

	int matrixWidth;
	int matrixHeight;
	int matrixWidthScaled;
	int matrixHeightScaled;

	enum ReceivingDataType { RECEIVING_NONE, RECEIVING_FRAME, RECEIVING_LOOKUP } receivingDataType;
	virtual void resetState() override;

	void setPixel(int col, int row, int matrixWidth, int matrixHeight, int startIdx, CRGB color);

	byte* buffer;

	int inputScale;
	int colorDepth;
	FlipAxisTypes flipAxis;

	PixelFormat pixelFormat;
	int pixelFormatMultiplier;

	CRGB lookupTable[256];

	int bytesReceived;
	RGBLedController& controller;
};

#endif // RGBLED_H
