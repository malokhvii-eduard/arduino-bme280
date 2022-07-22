#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_

#include <math.h>
#include <stdint.h>

/* Google Test */
#include <gmock/gmock.h>

#define HIGH 0x1
#define LOW 0x0

#define OUTPUT 0x01

#define LSBFIRST 0
#define MSBFIRST 1

unsigned long millis();

void delay(unsigned long);

void pinMode(uint8_t pin, uint8_t mode);

void digitalWrite(uint8_t pin, uint8_t val);

class ArduinoMock {
 public:
  MOCK_METHOD(unsigned long, millis, ());
  MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
  MOCK_METHOD(void, delay, (unsigned long));
  MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
};

ArduinoMock* createArduinoMock();
void destroyArduinoMock();

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_
