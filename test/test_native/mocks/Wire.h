#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_

#include <stdint.h>

/* Google Test */
#include <gmock/gmock.h>

class TwoWireMock {
 public:
  MOCK_METHOD(void, beginTransmission, (uint8_t));
  MOCK_METHOD(uint8_t, endTransmission, ());
  MOCK_METHOD(uint8_t, requestFrom, (uint8_t, uint8_t));
  MOCK_METHOD(size_t, write, (uint8_t));
  MOCK_METHOD(int, read, ());
};

using TwoWire = TwoWireMock;

extern TwoWire Wire;

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_
