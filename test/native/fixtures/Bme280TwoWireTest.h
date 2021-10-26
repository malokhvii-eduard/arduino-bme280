#ifndef ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_TWO_WIRE_TEST_H_
#define ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_TWO_WIRE_TEST_H_

/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Mocks */
#include <mocks/Bme280.h>

/* Test Parameters */
#include <params/Communication.h>

class Bme280TwoWireTest : public ::testing::Test {
 protected:
  ::testing::NiceMock<TwoWireMock> wire;
  Bme280TwoWireMock sensor;
};

class Bme280TwoWireTestWithAddress
    : public Bme280TwoWireTest,
      public ::testing::WithParamInterface<Bme280TwoWireAddress> {
 protected:
  void SetUp() override { address = static_cast<uint8_t>(GetParam()); }

  uint8_t address;
};

class Bme280TwoWireTestWithCommunication
    : public ::testing::TestWithParam<
          std::tuple<Bme280TwoWireAddress, uint8_t, CommunicationSnapshot>> {
 protected:
  void SetUp() override {
    sensor.wire_ = &wire;
    sensor.address_ = static_cast<uint8_t>(std::get<0>(GetParam()));

    registerAddress = std::get<1>(GetParam());
    communicationSnapshot = std::get<2>(GetParam());
    expectedValue = communicationSnapshot[0];
  }

  ::testing::StrictMock<TwoWireMock> wire;
  Bme280TwoWire sensor;

  uint8_t registerAddress;
  CommunicationSnapshot communicationSnapshot;
  long expectedValue;
};

class Bme280TwoWireTestWith8BitCommunication
    : public Bme280TwoWireTestWithCommunication {};

class Bme280TwoWireTestWith16BitCommunication
    : public Bme280TwoWireTestWithCommunication {};

class Bme280TwoWireTestWith24BitCommunication
    : public Bme280TwoWireTestWithCommunication {};

#endif  // ARDUINO_BME280_TEST_NATIVE_FIXTURES_BME280_TWO_WIRE_TEST_H_
