#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_

#include <math.h>
#include <stdint.h>

/* Google Test */
#include <gmock/gmock.h>

void delay(unsigned long);

class ArduinoMock {
 public:
  MOCK_METHOD(void, delay, (unsigned long));
};

ArduinoMock* createArduinoMock();
void destroyArduinoMock();

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_ARDUINO_H_
