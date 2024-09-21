#include "RGBLedController.h"
#include "RGBLed.h"

RGBLedController::RGBLedController()
	:
		dataPin(-1),
		numLeds(0),
		leds(nullptr)
	{}

RGBLedController::~RGBLedController() {
	delete[] leds;
}

void RGBLedController::setDataPin(int dataPinCustom) {
	dataPin = dataPinCustom;
}

void RGBLedController::initDataPin() {
	if (dataPin == 0) { FastLED.addLeds<WS2812B, DATA_PIN_00, GRB>(leds, numLeds); }
	else if (dataPin == 1) { FastLED.addLeds<WS2812B, DATA_PIN_01, GRB>(leds, numLeds); }
	else if (dataPin == 2) { FastLED.addLeds<WS2812B, DATA_PIN_02, GRB>(leds, numLeds); }
	else if (dataPin == 3) { FastLED.addLeds<WS2812B, DATA_PIN_03, GRB>(leds, numLeds); }
	else if (dataPin == 4) { FastLED.addLeds<WS2812B, DATA_PIN_04, GRB>(leds, numLeds); }
	else if (dataPin == 5) { FastLED.addLeds<WS2812B, DATA_PIN_05, GRB>(leds, numLeds); }
	else if (dataPin == 6) { FastLED.addLeds<WS2812B, DATA_PIN_06, GRB>(leds, numLeds); }
	else if (dataPin == 7) { FastLED.addLeds<WS2812B, DATA_PIN_07, GRB>(leds, numLeds); }
	else if (dataPin == 8) { FastLED.addLeds<WS2812B, DATA_PIN_08, GRB>(leds, numLeds); }
	else if (dataPin == 9) { FastLED.addLeds<WS2812B, DATA_PIN_09, GRB>(leds, numLeds); }
	else if (dataPin == 10) { FastLED.addLeds<WS2812B, DATA_PIN_10, GRB>(leds, numLeds); }
	else if (dataPin == 11) { FastLED.addLeds<WS2812B, DATA_PIN_11, GRB>(leds, numLeds); }
	else if (dataPin == 12) { FastLED.addLeds<WS2812B, DATA_PIN_12, GRB>(leds, numLeds); }
	else if (dataPin == 13) { FastLED.addLeds<WS2812B, DATA_PIN_13, GRB>(leds, numLeds); }

	FastLED.setBrightness(255);

	randomSeed(analogRead(0));
	fill_solid(leds, numLeds, CRGB(random(32), random(32), random(32)));

	update();
}

void RGBLedController::begin() {
	initDataPin();
}

void RGBLedController::updateTotalLength() {
	numLeds = 0;

	for (RGBLed* matrix : ledMatrices) {
		numLeds += matrix->getNumLeds();
	}

	delete[] leds;
	leds = new CRGB[numLeds];

	initDataPin();
}

void RGBLedController::attachMatrix(RGBLed* ledMatrix) {
	ledMatrices.push_back(ledMatrix);

	updateTotalLength();
}

void RGBLedController::update() {
	FastLED.show();
}

CRGB* RGBLedController::getLedArray() {
	return leds;
}

int RGBLedController::calculateLedIndex(int x, int y, int matrixWidth, int matrixHeight, int startIdx) {
	int index = startIdx + ((y % 2 == 0) ? (y * matrixWidth + x) : (y * matrixWidth + (matrixWidth - 1 - x)));

	return index;
}

int RGBLedController::getStartIndexForMatrix(RGBLed* matrix) {
	int startIndex = 0;

	for (RGBLed* mat : ledMatrices) {
		if (mat == matrix) break;
		startIndex += mat->getNumLeds();
	}

	return startIndex;
}
