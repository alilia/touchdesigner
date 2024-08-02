#include "FanControl.h"

FanControl::FanControl() {
  state = WAITING_FOR_DATA;
  fanSpeed = 0; // default fan speed
}

void FanControl::begin() {
  begin(0xFD); // default frame marker
}

void FanControl::begin(int fanSpeedMarkerCustom) {
  pinMode(DATA_PIN, OUTPUT);
  fanSpeedMarker = fanSpeedMarkerCustom;
  fanSpeed = 0;
  publishFanSpeed();
}

void FanControl::publishFanSpeed() {
  analogWrite(DATA_PIN, fanSpeed);
}

void FanControl::receiveSerialData() {
  while (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    Serial.println(incomingByte);
    processIncomingByte(incomingByte);
  }
}

void FanControl::processIncomingByte(byte incomingByte) {
  if (state == WAITING_FOR_DATA) {
    if (incomingByte == fanSpeedMarker) {
      state = RECEIVING_DATA;
    }
  } else if (state == RECEIVING_DATA) {
    fanSpeed = incomingByte;
    publishFanSpeed();
    state = WAITING_FOR_DATA;
  }
}
