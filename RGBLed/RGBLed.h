#ifndef RGBLED_H
#define RGBLED_H

#include <FastLED.h>

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define DATA_PIN 6

class RGBLed {
public:
	RGBLed();
	void begin();
	void begin(int frameMarkerCustom);
	void setPixel(int x, int y, CRGB color);
	void setBrightness(uint8_t brightness);
	void update();
	void receiveSerialData();

private:
	CRGB leds[NUM_LEDS];
	enum State { WAITING_FOR_DATA, RECEIVING_DATA } state;
	int currentRow;
	int bytesReceived;
	byte buffer[3];
	void processIncomingByte(byte incomingByte);
	int frameMarker;
};

#endif
