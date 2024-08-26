#include "RGBLed.h"

RGBLed::RGBLed(int matrixWidthCustom, int matrixHeightCustom, int rgbResolution, int inputMarkerCustom, int outputDataPinCustom)
	: TDComm(-1, inputMarkerCustom, outputDataPinCustom, -1, 1000),
		matrixWidth(matrixWidthCustom),
		matrixHeight(matrixHeightCustom),
		numLeds( matrixWidth * matrixHeight ),
		rgbResolution(rgbResolution),
		bytesReceived(0),
		currentRow(0)
	{
		numLeds = matrixWidth * matrixHeight;
		leds = new CRGB[numLeds];
		buffer = new byte[numLeds * 3];
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

	fill_solid(leds, numLeds, CRGB(random(32), random(32), random(32)));

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

	int expectingBytes = numLeds * 3 * rgbResolution / 8; // since python is packing 4x6-bit values to 3x8-bits

	if (bytesReceived == expectingBytes) {
		int unpackedValues[numLeds * 3];
		unpack_values(buffer, unpackedValues, bytesReceived, rgbResolution);

		for (int row = 0; row < matrixHeight; row++) {
			for (int col = 0; col < matrixWidth; col++) {
				int max_value = (1 << rgbResolution) - 1;

				int idx = ( row * matrixWidth + col ) * 3;
				int r = unpackedValues[idx] * 255 / max_value;
				int g = unpackedValues[idx + 1] * 255 / max_value;
				int b = unpackedValues[idx + 2] * 255 / max_value;

				setPixel(col, row, CRGB(r, g, b));
			}
		}

		bytesReceived = 0;
		update();
		resetState();
	}
}

void RGBLed::unpack_values(byte* buffer, int* values, int length, int bitsPerValue) {
	int bitMask = (1 << bitsPerValue) - 1;
	int bitsAvailable = 0;
	int temp = 0;

	int valueIndex = 0;

	for (int i = 0; i < length; i++) {
		temp = (temp << 8) | buffer[i];
		bitsAvailable += 8;

		while (bitsAvailable >= bitsPerValue) {
			bitsAvailable -= bitsPerValue;
			values[valueIndex++] = (temp >> bitsAvailable) & bitMask;
		}
	}

	if (bitsAvailable > 0 && valueIndex < length) {
		values[valueIndex] = (temp << (bitsPerValue - bitsAvailable)) & bitMask;
	}
}
