#ifndef ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_FOUR_WIRE_TEST_H_
#define ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_FOUR_WIRE_TEST_H_

/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Mocks */
#include <mocks/Arduino.h>
#include <mocks/Bme280.h>

/* Test Parameters */
#include <params/Communication.h>

class Bme280FourWireTestWithCsPin : public ::testing::TestWithParam<uint8_t> {
 protected:
  void SetUp() override {
    arduino = createArduinoMock();
    csPin = static_cast<uint8_t>(GetParam());
  }

  void TearDown() override { destroyArduinoMock(); }

  ArduinoMock* arduino;
  ::testing::NiceMock<SpiMock> spi;
  Bme280FourWireMock sensor;

  uint8_t csPin;
};

class Bme280FourWireTestWithCommunication
    : public ::testing::TestWithParam<
          std::tuple<uint8_t, uint8_t, CommunicationSnapshot>> {
 protected:
  void SetUp() override {
    arduino = createArduinoMock();

    sensor.spi_ = &spi;
    sensor.csPin_ = static_cast<uint8_t>(std::get<0>(GetParam()));

    registerAddress = std::get<1>(GetParam());
    communicationSnapshot = std::get<2>(GetParam());
    expectedValue = communicationSnapshot[0];
  }

  void TearDown() override { destroyArduinoMock(); }

  ArduinoMock* arduino;
  ::testing::StrictMock<SpiMock> spi;
  Bme280FourWire sensor;

  uint8_t registerAddress;
  CommunicationSnapshot communicationSnapshot;
  long expectedValue;
};

class Bme280FourWireTestWith8BitCommunication
    : public Bme280FourWireTestWithCommunication {};

class Bme280FourWireTestWith16BitCommunication
    : public Bme280FourWireTestWithCommunication {};

class Bme280FourWireTestWith24BitCommunication
    : public Bme280FourWireTestWithCommunication {};

#endif  // ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_FOUR_WIRE_TEST_H_
