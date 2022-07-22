#include <mocks/Arduino.h>

ArduinoMock* arduino = nullptr;

unsigned long millis() {
  assert(arduino != nullptr);
  return arduino->millis();
}

void delay(unsigned long ms) {
  assert(arduino != nullptr);
  arduino->delay(ms);
}

void pinMode(uint8_t pin, uint8_t mode) {
  assert(arduino != nullptr);
  arduino->pinMode(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t val) {
  assert(arduino != nullptr);
  arduino->digitalWrite(pin, val);
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
