#include "RGBLed.h"

RGBLed::RGBLed(int matrixWidthCustom, int matrixHeightCustom, int inputMarkerCustom, int outputDataPinCustom)
	: TDComm(-1, inputMarkerCustom, outputDataPinCustom, -1, 1000),
		matrixWidth(matrixWidthCustom),
		matrixHeight(matrixHeightCustom),
		numLeds( matrixWidth * matrixHeight ),
		bytesReceived(0),
		currentRow(0)
	{
		numLeds = matrixWidth * matrixHeight;
		leds = new CRGB[numLeds];
		buffer = new byte[matrixWidth * 3];
	}

RGBLed::~RGBLed() {
	delete[] leds;
	delete[] buffer;
}

void RGBLed::begin() {
	randomSeed(analogRead(0));

	if (outputDataPin == 0) { FastLED.addLeds<WS2812B, DATA_PIN_00, GRB>(leds, numLeds); }
	else if (outputDataPin == 1) { FastLED.addLeds<WS2812B, DATA_PIN_01, GRB>(leds, numLeds); }
	else if (outputDataPin == 2) { FastLED.addLeds<WS2812B, DATA_PIN_02, GRB>(leds, numLeds); }
	else if (outputDataPin == 3) { FastLED.addLeds<WS2812B, DATA_PIN_03, GRB>(leds, numLeds); }
	else if (outputDataPin == 4) { FastLED.addLeds<WS2812B, DATA_PIN_04, GRB>(leds, numLeds); }
	else if (outputDataPin == 5) { FastLED.addLeds<WS2812B, DATA_PIN_05, GRB>(leds, numLeds); }
	else if (outputDataPin == 6) { FastLED.addLeds<WS2812B, DATA_PIN_06, GRB>(leds, numLeds); }
	else if (outputDataPin == 7) { FastLED.addLeds<WS2812B, DATA_PIN_07, GRB>(leds, numLeds); }
	else if (outputDataPin == 8) { FastLED.addLeds<WS2812B, DATA_PIN_08, GRB>(leds, numLeds); }
	else if (outputDataPin == 9) { FastLED.addLeds<WS2812B, DATA_PIN_09, GRB>(leds, numLeds); }
	else if (outputDataPin == 10) { FastLED.addLeds<WS2812B, DATA_PIN_10, GRB>(leds, numLeds); }
	else if (outputDataPin == 11) { FastLED.addLeds<WS2812B, DATA_PIN_11, GRB>(leds, numLeds); }
	else if (outputDataPin == 12) { FastLED.addLeds<WS2812B, DATA_PIN_12, GRB>(leds, numLeds); }
	else if (outputDataPin == 13) { FastLED.addLeds<WS2812B, DATA_PIN_13, GRB>(leds, numLeds); }

	FastLED.setBrightness(255);

	fill_solid(leds, numLeds, CRGB(random(256), random(256), random(256)));

	update();
}

void RGBLed::setPixel(int x, int y, CRGB color) {
	if (x >= 0 && x < matrixWidth && y >= 0 && y < matrixHeight) {
		int ledIndex = (y % 2 == 0) ? (y * matrixWidth + x) : (y * matrixWidth + (matrixWidth - 1 - x));
		leds[ledIndex] = color;
	}
}

void RGBLed::update() {
	FastLED.show();
}

void RGBLed::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
			currentRow = 0;
			bytesReceived = 0;
			lastMiliSec = millis();
		}
	} else if (state == RECEIVING_DATA) {
		processIncomingByte(incomingByte);
	}
}

void RGBLed::processIncomingByte(byte incomingByte) {
	buffer[bytesReceived++] = incomingByte;

	if (bytesReceived == matrixWidth * 3) {
		for (int col = 0; col < matrixWidth; col++) {
			int r = buffer[col * 3];
			int g = buffer[col * 3 + 1];
			int b = buffer[col * 3 + 2];

			setPixel(col, currentRow, CRGB(r, g, b));
		}

		bytesReceived = 0;
		currentRow++;

		if (currentRow == matrixHeight) {
			update();
			resetState();
		}
	}
}
