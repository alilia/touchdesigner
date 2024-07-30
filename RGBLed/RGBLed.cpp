#include "RGBLed.h"

RGBLed::RGBLed() {
    state = WAITING_FOR_DATA;
    currentRow = 0;
    bytesReceived = 0;
}

void RGBLed::begin() {
    begin(0xFF); // default frame marker
}

void RGBLed::begin(int frameMarkerCustom) {
    frameMarker = frameMarkerCustom;
    randomSeed(analogRead(0));
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
    fill_solid(leds, NUM_LEDS, CRGB(random(256), random(256), random(256)));
    update();
}

void RGBLed::setPixel(int x, int y, CRGB color) {
    if (x >= 0 && x < MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT) {
        int ledIndex = (y % 2 == 0) ? (y * MATRIX_WIDTH + x) : (y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x));
        leds[ledIndex] = color;
    }
}

void RGBLed::update() {
    FastLED.show();
}

void RGBLed::receiveSerialData() {
    while (Serial.available() > 0) {
        byte incomingByte = Serial.read();
        processIncomingByte(incomingByte);
    }
}

void RGBLed::processIncomingByte(byte incomingByte) {
  static byte buffer[MATRIX_WIDTH * 3]; // Buffer to store one row of data

  if (state == WAITING_FOR_DATA) {
    if (incomingByte == frameMarker) { // Start of new frame marker
      state = RECEIVING_DATA;
      currentRow = 0;
      bytesReceived = 0;
    }
  } else if (state == RECEIVING_DATA) {
    buffer[bytesReceived++] = incomingByte;

    if (bytesReceived == MATRIX_WIDTH * 3) {
      // Process the buffer
      for (int col = 0; col < MATRIX_WIDTH; col++) {
        int r = buffer[col * 3];
        int g = buffer[col * 3 + 1];
        int b = buffer[col * 3 + 2];

        int ledIndex = currentRow * MATRIX_WIDTH + col;
        setPixel(col, currentRow, CRGB(r, g, b));
      }

      // Reset buffer for next row
      bytesReceived = 0;
      currentRow++;

      // Check if we have received the entire frame
      if (currentRow == MATRIX_HEIGHT) {
        update();
        state = WAITING_FOR_DATA;
      }
    }
  }
}
