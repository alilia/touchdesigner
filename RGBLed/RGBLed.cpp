#include "RGBLed.h"

RGBLed::RGBLed()
	:
		bytesReceived(0),
		receivingDataType(RECEIVING_NONE),
		inputScale(1),
		pixelFormatMultiplier(3)
	{
		// filling table for debug
		for (int i = 0; i<256; i++) {
			lookupTable[i] = CRGB(i, 0, 0);
		}
	}

RGBLed::~RGBLed() {
	delete[] leds;
	delete[] buffer;
}

void RGBLed::setResolution(int matrixWidthCustom, int matrixHeightCustom) {
	matrixWidth = matrixWidthCustom;
	matrixHeight = matrixHeightCustom;

	matrixWidthScaled = matrixWidth / inputScale;
	matrixHeightScaled = matrixHeight / inputScale;

	numLeds = matrixWidth * matrixHeight;

	leds = new CRGB[numLeds];
	buffer = new byte[numLeds * 3]; // even if mono, lookup is rgb
}

void RGBLed::setPixelFormat(PixelFormat pixelFormatCustom) {
	pixelFormat = pixelFormatCustom; // bitpacking

	if (pixelFormat == PIXEL_FORMAT_RGB) pixelFormatMultiplier = 3;
	else if (pixelFormat == PIXEL_FORMAT_MONO) pixelFormatMultiplier = 1;
}

void RGBLed::setColorDepth(int colorDepthCustom) {
	colorDepth = colorDepthCustom;
}

void RGBLed::setInputScale(int inputScaleCustom) {
	if (!inputScaleCustom) inputScaleCustom = 0.5;

	inputScale = inputScaleCustom;
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

int RGBLed::calculateLedIndex(int x, int y) {
	return (y % 2 == 0) ? (y * matrixWidth + x) : (y * matrixWidth + (matrixWidth - 1 - x));
}

void RGBLed::setPixel(int x, int y, CRGB color) {
	if (x >= 0 && x < matrixWidthScaled && y >= 0 && y < matrixHeightScaled) {
		if (inputScale > 1) {
			x *= inputScale;
			y *= inputScale;

			for (int i = 0; i < inputScale; i++) {
				for (int j = 0; j < inputScale; j++) {
					leds[calculateLedIndex(x + i, y + j)] = color;
				}
			}
		} else {
			leds[calculateLedIndex(x, y)] = color;
		}
	}
}

void RGBLed::update() {
	FastLED.show();
}

void RGBLed::receiveSerialData(byte incomingByte) {
	checkStateTimeout();

	switch(state) {
		case WAITING_FOR_MARKER:
			if (incomingByte == inputMarker) {
				state = RECEIVING_DATA;
				bytesReceived = 0;
				lastMiliSec = millis();
			}
			break;

		case RECEIVING_DATA:
			switch(receivingDataType) {
				case RECEIVING_NONE:
					if (incomingByte == 0xFF) receivingDataType = RECEIVING_FRAME;
					else if (incomingByte == 0xFE) receivingDataType = RECEIVING_LOOKUP;
					else resetState();
					break;

				case RECEIVING_FRAME:
					processIncomingByteFrame(incomingByte);
					break;

				case RECEIVING_LOOKUP:
					processIncomingByteLookup(incomingByte);
					break;
			}

			break;
	}
}

void RGBLed::processIncomingByteLookup(byte incomingByte) {
	buffer[bytesReceived++] = incomingByte;

	int expectingBytes = 256 * 3;

	if (bytesReceived == expectingBytes) {
		for (int i = 0; i < 256; i++) {
			int idx = i * 3;

			int r = buffer[idx];
			int g = buffer[idx + 1];
			int b = buffer[idx + 2];

			lookupTable[i] = CRGB(r, g, b);
		}

		resetState();
	}
}

void RGBLed::processIncomingByteFrame(byte incomingByte) {
	buffer[bytesReceived++] = incomingByte;

	// base led number
	int expectingBytes = numLeds;

	// mono or rgb
	expectingBytes *= pixelFormatMultiplier;

	// ratio based on bitpacking
	expectingBytes *= colorDepth;
	expectingBytes /= 8;

	// scaling down the expectation
	expectingBytes /= pow(inputScale, inputScale);

	if (bytesReceived == expectingBytes) {
		int unpackedValues[numLeds * pixelFormatMultiplier];
		unpack_values(buffer, unpackedValues, bytesReceived, colorDepth);

		for (int row = 0; row < matrixHeightScaled; row++) {
			for (int col = 0; col < matrixWidthScaled; col++) {
				int max_value = (1 << colorDepth) - 1;

				int idx = ( row * matrixWidthScaled + col ) * pixelFormatMultiplier;

				if (pixelFormat == PIXEL_FORMAT_RGB) {
					int r = unpackedValues[idx] * 255 / max_value;
					int g = unpackedValues[idx + 1] * 255 / max_value;
					int b = unpackedValues[idx + 2] * 255 / max_value;

					setPixel(col, row, CRGB(r, g, b));
				} else if (pixelFormat == PIXEL_FORMAT_MONO) {
					int m = unpackedValues[idx] * 255 / max_value;

					setPixel(col, row, lookupTable[m]);
				}
			}
		}

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

void RGBLed::resetState() {
	TDComm::resetState();
	bytesReceived = 0;
	receivingDataType = RECEIVING_NONE;
}
