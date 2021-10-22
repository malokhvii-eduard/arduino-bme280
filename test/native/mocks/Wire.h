#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_

#include <stdint.h>

/* Google Test */
#include <gmock/gmock.h>

class TwoWire {
 public:
  virtual ~TwoWire();

  virtual void beginTransmission(uint8_t);
  virtual uint8_t endTransmission();
  virtual uint8_t requestFrom(uint8_t, uint8_t);
  virtual size_t write(uint8_t);
  virtual int read();
};

class TwoWireMock : public TwoWire {
 public:
  MOCK_METHOD(void, beginTransmission, (uint8_t), (override));
  MOCK_METHOD(uint8_t, endTransmission, (), (override));
  MOCK_METHOD(uint8_t, requestFrom, (uint8_t, uint8_t), (override));
  MOCK_METHOD(size_t, write, (uint8_t), (override));
  MOCK_METHOD(int, read, (), (override));
};

extern TwoWireMock Wire;

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_WIRE_H_
