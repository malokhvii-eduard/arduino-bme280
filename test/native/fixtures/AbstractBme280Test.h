#ifndef ARDUINO_BME280_TEST_NATIVE_FIXTURES_ABSTRACT_BME280_TEST_H_
#define ARDUINO_BME280_TEST_NATIVE_FIXTURES_ABSTRACT_BME280_TEST_H_

#include <array>

/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Mocks */
#include <mocks/Arduino.h>
#include <mocks/Bme280.h>

/* Test Parameters */
#include <params/Communication.h>

using SettingsSnapshot = std::array<uint8_t, 3>;

class AbstractBme280Test : public ::testing::Test {
 protected:
  void SetUp() override {
    arduino = createArduinoMock();
    sensor.calibrationData_ = expectedCalibrationData;
  }

  void TearDown() override { destroyArduinoMock(); }

  ArduinoMock* arduino;
  ::testing::NiceMock<AbstractBme280Mock> sensor;
  Bme280CalibrationData expectedCalibrationData = {.digT1 = 28780,
                                                   .digT2 = 26513,
                                                   .digT3 = 50,
                                                   .digP1 = 35660,
                                                   .digP2 = -10546,
                                                   .digP3 = 3024,
                                                   .digP4 = 5700,
                                                   .digP5 = 14,
                                                   .digP6 = -7,
                                                   .digP7 = 9900,
                                                   .digP8 = -10230,
                                                   .digP9 = 4285,
                                                   .digH1 = 75,
                                                   .digH2 = 357,
                                                   .digH3 = 0,
                                                   .digH4 = 332,
                                                   .digH5 = 50,
                                                   .digH6 = 30};
};

class AbstractBme280TestAlt : public ::testing::Test {
 protected:
  void SetUp() override { arduino = createArduinoMock(); }
  void TearDown() override { destroyArduinoMock(); }

  ArduinoMock* arduino;
  ::testing::NiceMock<AbstractBme280MockAlt> sensor;
};

class AbstractBme280TestWithChipId
    : public AbstractBme280Test,
      public ::testing::WithParamInterface<uint8_t> {
  void SetUp() override { sensor.chipId_ = GetParam(); }
};

class AbstractBme280TestWithSettings
    : public AbstractBme280Test,
      public ::testing::WithParamInterface<
          std::tuple<Bme280Settings, SettingsSnapshot>> {
 protected:
  void SetUp() override {
    AbstractBme280Test::SetUp();

    expectedSettings = std::get<0>(GetParam());
    settingsSnapshot = std::get<1>(GetParam());
  }

  SettingsSnapshot settingsSnapshot;
  Bme280Settings expectedSettings;
};

class AbstractBme280TwoWireTestWithCommunication
    : public ::testing::TestWithParam<
          std::tuple<uint8_t, CommunicationSnapshot>> {
 protected:
  void SetUp() override {
    registerAddress = std::get<0>(GetParam());
    communicationSnapshot = std::get<1>(GetParam());
    expectedValue = communicationSnapshot[0];
  }

  ::testing::StrictMock<AbstractBme280Mock> sensor;

  uint8_t registerAddress;
  CommunicationSnapshot communicationSnapshot;
  long expectedValue;
};

class AbstractBme280TestWith16BitCommunication
    : public AbstractBme280TwoWireTestWithCommunication {};

class AbstractBme280TestWith16BitLittleEndianCommunication
    : public AbstractBme280TwoWireTestWithCommunication {};

#endif  // ARDUINO_BME280_TEST_NATIVE_FIXTURES_ABSTRACT_BME280_TEST_H_
