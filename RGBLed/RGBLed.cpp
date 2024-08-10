#include "RGBLed.h"

#define DATA_PIN 6

RGBLed::RGBLed(int matrixWidthCustom, int matrixHeightCustom, int inputDataPinCustom, int inputMarkerCustom, int outputDataPinCustom, int outputMarkerCustom)
	: TDComm(inputDataPinCustom, inputMarkerCustom, outputDataPinCustom, outputMarkerCustom),
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
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, numLeds); // DATA_PIN is a defined constant
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
	if (state == WAITING_FOR_DATA) {
		if (incomingByte == inputMarker) {
			state = RECEIVING_DATA;
			currentRow = 0;
			bytesReceived = 0;
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
			state = WAITING_FOR_DATA;
		}
	}
}

void RGBLed::loop() {}
