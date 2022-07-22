#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_BME280_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_BME280_H_

/* Google Test */
#include <gmock/gmock.h>

/* Library */
#include <Bme280.h>

class AbstractBme280Mock : public ::internal::AbstractBme280 {
 public:
  MOCK_METHOD(void, write8, (uint8_t, uint8_t), (override));
  MOCK_METHOD(uint8_t, read8, (uint8_t), (const, override));
  MOCK_METHOD(uint16_t, read16, (uint8_t), (const, override));
  MOCK_METHOD(uint32_t, read24, (uint8_t), (const, override));
};

class AbstractBme280MockAlt : public AbstractBme280Mock {
 public:
  MOCK_METHOD(void, reset, (), (override));
  MOCK_METHOD(bool, isImUpdate, (), (const, override));
  MOCK_METHOD(void, readCalibrationData, (), (const, override));
  MOCK_METHOD(void, setSettings, (const Bme280Settings), (override));
};

class Bme280TwoWireMock : public Bme280TwoWire {
 public:
  MOCK_METHOD(bool, setup, (), (override));
};

class Bme280FourWireMock : public Bme280FourWire {
 public:
  MOCK_METHOD(bool, setup, (), (override));
};

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_BME280_H_
