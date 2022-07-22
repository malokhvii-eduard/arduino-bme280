/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Fixtures */
#include <fixtures/Bme280FourWireTest.h>

using ::testing::Combine;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Values;

bool operator==(const Bme280FourWire::ControlByte& l,
                const Bme280FourWire::ControlByte& r) {
  return l.value == r.value;
}

TEST_P(Bme280FourWireTestWithCsPin, BeginsWithCsPinAndWithoutSpi) {
  {
    InSequence s;
    EXPECT_CALL(*arduino, pinMode(csPin, OUTPUT));
    EXPECT_CALL(*arduino, digitalWrite(csPin, HIGH));
    EXPECT_CALL(sensor, setup()).WillOnce(Return(true));
  }

  EXPECT_TRUE(sensor.begin(csPin));
  EXPECT_EQ(sensor.spi_, &SPI);
  EXPECT_EQ(sensor.csPin_, csPin);
}

TEST_P(Bme280FourWireTestWithCsPin, BeginsWithCsPinAndSpi) {
  {
    InSequence s;
    EXPECT_CALL(*arduino, pinMode(csPin, OUTPUT));
    EXPECT_CALL(*arduino, digitalWrite(csPin, HIGH));
    EXPECT_CALL(sensor, setup()).WillOnce(Return(true));
  }

  EXPECT_TRUE(sensor.begin(csPin, &spi));
  EXPECT_EQ(sensor.spi_, &spi);
  EXPECT_EQ(sensor.csPin_, csPin);
}

TEST_P(Bme280FourWireTestWithCsPin, GetsCsPin) {
  sensor.csPin_ = csPin;
  EXPECT_EQ(sensor.getCsPin(), sensor.csPin_);
}

TEST_P(Bme280FourWireTestWith8BitCommunication, Writes) {
  Bme280FourWire::ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 0;

  {
    InSequence s;
    EXPECT_CALL(spi, beginTransaction(Bme280FourWire::spiSettings_));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, LOW));
    EXPECT_CALL(spi, transfer(control.value));
    EXPECT_CALL(spi, transfer(expectedValue));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, HIGH));
    EXPECT_CALL(spi, endTransaction());
  }

  sensor.write8(registerAddress, expectedValue);
}

TEST_P(Bme280FourWireTestWith8BitCommunication, Reads) {
  Bme280FourWire::ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  {
    InSequence s;
    EXPECT_CALL(spi, beginTransaction(Bme280FourWire::spiSettings_));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, LOW));
    EXPECT_CALL(spi, transfer(control.value));
    EXPECT_CALL(spi, transfer(0)).WillOnce(Return(expectedValue));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, HIGH));
    EXPECT_CALL(spi, endTransaction());
  }

  EXPECT_EQ(sensor.read8(registerAddress), expectedValue);
}

TEST_P(Bme280FourWireTestWith16BitCommunication, Reads) {
  Bme280FourWire::ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  {
    InSequence s;
    EXPECT_CALL(spi, beginTransaction(Bme280FourWire::spiSettings_));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, LOW));
    EXPECT_CALL(spi, transfer(control.value));
    EXPECT_CALL(spi, transfer(0))
        .WillOnce(Return(communicationSnapshot[1]))
        .WillOnce(Return(communicationSnapshot[2]));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, HIGH));
    EXPECT_CALL(spi, endTransaction());
  }

  EXPECT_EQ(sensor.read16(registerAddress), expectedValue);
}

TEST_P(Bme280FourWireTestWith24BitCommunication, Reads) {
  Bme280FourWire::ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  {
    InSequence s;
    EXPECT_CALL(spi, beginTransaction(Bme280FourWire::spiSettings_));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, LOW));
    EXPECT_CALL(spi, transfer(control.value));
    EXPECT_CALL(spi, transfer(0))
        .WillOnce(Return(communicationSnapshot[1]))
        .WillOnce(Return(communicationSnapshot[2]))
        .WillOnce(Return(communicationSnapshot[3]));
    EXPECT_CALL(*arduino, digitalWrite(sensor.csPin_, HIGH));
    EXPECT_CALL(spi, endTransaction());
  }

  EXPECT_EQ(sensor.read24(registerAddress), expectedValue);
}

#define CS_PIN Values(3u, 5u)

INSTANTIATE_TEST_SUITE_P(CsPins, Bme280FourWireTestWithCsPin, CS_PIN);

INSTANTIATE_TEST_SUITE_P(P_8_BIT_MESSAGES,
                         Bme280FourWireTestWith8BitCommunication,
                         Combine(CS_PIN, PARAM_8_BIT_REGISTER,
                                 PARAM_8_BIT_COMMUNICATION_SNAPSHOT));

INSTANTIATE_TEST_SUITE_P(P_16_BIT_MESSAGES,
                         Bme280FourWireTestWith16BitCommunication,
                         Combine(CS_PIN, PARAM_16_BIT_REGISTER,
                                 PARAM_16_BIT_COMMUNICATION_SNAPSHOT));

INSTANTIATE_TEST_SUITE_P(P_24_BIT_MESSAGES,
                         Bme280FourWireTestWith24BitCommunication,
                         Combine(CS_PIN, PARAM_24_BIT_REGISTER,
                                 PARAM_24_BIT_COMMUNICATION_SNAPSHOT));
