#ifndef RGBLEDCONTROLLER_H
#define RGBLEDCONTROLLER_H

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

class RGBLed;

class RGBLedController {
public:
	RGBLedController();
	~RGBLedController();

	void setDataPin(int dataPin);
	void initDataPin();
	void begin();
	void update();

	void addLedMatrix(RGBLed* ledMatrix);

	void attachMatrix(RGBLed* ledMatrix);
	CRGB* getLedArray();
	int calculateLedIndex(int x, int y, int matrixWidth, int matrixHeight, int startIdx);
	int getStartIndexForMatrix(RGBLed* matrix);

private:
	RGBLed** ledMatrices = nullptr;   // Pointer to the array of RGBLed pointers
	int ledMatricesSize = 0;          // Number of elements currently in the array
	int ledMatricesCapacity = 0;      // The allocated size of the array

	int numLeds;
	int dataPin;

	CRGB* leds;

	void updateTotalLength();
};

#endif // RGBLEDCONTROLLER_H
