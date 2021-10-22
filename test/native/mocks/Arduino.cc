#include <mocks/Arduino.h>

ArduinoMock* arduino = nullptr;

void delay(unsigned long ms) {
  assert(arduino != nullptr);
  arduino->delay(ms);
}

ArduinoMock* createArduinoMock() {
  if (!arduino) {
    arduino = new ArduinoMock();
  }

  return arduino;
}

void destroyArduinoMock() {
  if (arduino) {
    delete arduino;
    arduino = nullptr;
  }
}
