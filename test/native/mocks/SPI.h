#ifndef ARDUINO_BME280_TEST_NATIVE_MOCKS_SPI_H_
#define ARDUINO_BME280_TEST_NATIVE_MOCKS_SPI_H_

#include <stdint.h>

/* Google Test */
#include <gmock/gmock.h>

/* Mocks */
#include <mocks/Arduino.h>

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x10
#define SPI_MODE3 0x11

struct SPISettings {
  SPISettings() : _clock(1000000), _bitOrder(LSBFIRST), _dataMode(SPI_MODE0) {}
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
      : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}

  bool operator==(const SPISettings& other) const;

  uint32_t _clock;
  uint8_t _bitOrder;
  uint8_t _dataMode;
};

class SpiMock {
 public:
  MOCK_METHOD(void, beginTransaction, (SPISettings));
  MOCK_METHOD(uint8_t, endTransaction, ());
  MOCK_METHOD(uint8_t, transfer, (uint8_t));
};

using SPIClass = SpiMock;

extern SPIClass SPI;

#endif  // ARDUINO_BME280_TEST_NATIVE_MOCKS_SPI_H_
