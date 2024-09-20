#include "RGBLed.h"

RGBLed::RGBLed(RGBLedController& controllerCustom)
	:
		controller(controllerCustom),
		buffer(nullptr)
	{
		setPixelFormat(PIXEL_FORMAT_RGB);
		setFlipAxis(FLIP_AXIS_NONE);
		setInputScale(1);
		setColorDepth(8);

		resetState();

		for (int i = 0; i < 256; i++) {
			lookupTable[i] = CRGB(i, 0, 0);
		}
	}

RGBLed::~RGBLed() {
	delete[] buffer;
}

void RGBLed::setResolution(int matrixWidthCustom, int matrixHeightCustom) {
	matrixWidth = matrixWidthCustom;
	matrixHeight = matrixHeightCustom;

	buffer = new byte[(matrixWidth / inputScale) * (matrixHeight / inputScale) * 3]; // even if mono, lookup is rgb

	matrixWidthScaled = matrixWidth / inputScale;
	matrixHeightScaled = matrixHeight / inputScale;

	controller.attachMatrix(this);
}

int RGBLed::getNumLeds() const {
	return matrixWidth * matrixHeight;
}

void RGBLed::setPixelFormat(PixelFormat pixelFormatCustom) {
	pixelFormat = pixelFormatCustom;
	pixelFormatMultiplier = (pixelFormat == PIXEL_FORMAT_RGB) ? 3 : 1;
}

void RGBLed::setColorDepth(int colorDepthCustom) {
	colorDepth = colorDepthCustom;
}

void RGBLed::setInputScale(int inputScaleCustom) {
	if (inputScaleCustom < 1) inputScaleCustom = 1;

	inputScale = inputScaleCustom;
}

void RGBLed::setFlipAxis(FlipAxisTypes flipAxisCustom) {
	flipAxis = flipAxisCustom;
}

void RGBLed::begin() {
	controller.begin();
}

void RGBLed::setPixel(int col, int row, int matrixWidth, int matrixHeight, int startIdx, CRGB color) {
	controller.getLedArray()[
		controller.calculateLedIndex(
			col, row, matrixWidth, matrixHeight, startIdx
		)
	] = color;
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

	int expectingBytes = (matrixWidthScaled * matrixHeightScaled) * pixelFormatMultiplier * colorDepth / 8;

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
	int expectingBytes = getNumLeds();

	// mono or rgb
	expectingBytes *= pixelFormatMultiplier;

	// ratio based on bitpacking
	expectingBytes *= colorDepth;
	expectingBytes /= 8;

	// scaling down the expectation
	expectingBytes /= pow(inputScale, 2);

	if (bytesReceived == expectingBytes) {
		int unpackedValues[getNumLeds() * pixelFormatMultiplier];
		unpack_values(buffer, unpackedValues, bytesReceived, colorDepth);

		int startIdx = controller.getStartIndexForMatrix(this);

		for (int row = 0; row < matrixHeight; row++) {
			for (int col = 0; col < matrixWidth; col++) {
				int max_value = (1 << colorDepth) - 1;
				int idx = (row * matrixWidth + col) * pixelFormatMultiplier;

				if (pixelFormat == PIXEL_FORMAT_RGB) {
					int r = unpackedValues[idx] * 255 / max_value;
					int g = unpackedValues[idx + 1] * 255 / max_value;
					int b = unpackedValues[idx + 2] * 255 / max_value;

					for (int i = 0; i < inputScale; i++) {
						for (int j = 0; j < inputScale; j++) {
							int x = col * inputScale + i;
							int y = row * inputScale + j;

							setPixel(x, y, matrixWidth, matrixHeight, startIdx, CRGB(r, g, b));
						}
					}
				} else if (pixelFormat == PIXEL_FORMAT_MONO) {
					int m = unpackedValues[idx] * 255 / max_value;

					for (int i = 0; i < inputScale; i++) {
						for (int j = 0; j < inputScale; j++) {
							int x = col * inputScale + i;
							int y = row * inputScale + j;

							setPixel(x, y, matrixWidth, matrixHeight, startIdx, lookupTable[m]);
						}
					}
				}
			}

			controller.update();
			resetState();
		}
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
