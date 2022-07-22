/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Fixtures */
#include <fixtures/Bme280TwoWireTest.h>

using ::testing::Combine;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Values;

TEST_F(Bme280TwoWireTest, BeginsWithoutAddressAndWire) {
  EXPECT_CALL(sensor, setup()).WillOnce(Return(true));

  EXPECT_TRUE(sensor.begin());
  EXPECT_EQ(sensor.wire_, &Wire);
  EXPECT_EQ(sensor.address_,
            static_cast<uint8_t>(Bme280TwoWireAddress::Primary));
}

TEST_P(Bme280TwoWireTestWithAddress, BeginsWithAddressAndWithoutWire) {
  EXPECT_CALL(sensor, setup()).WillOnce(Return(true));

  EXPECT_TRUE(sensor.begin(GetParam()));
  EXPECT_EQ(sensor.wire_, &Wire);
  EXPECT_EQ(sensor.address_, address);
}

TEST_P(Bme280TwoWireTestWithAddress, BeginsWithAddressAndWire) {
  EXPECT_CALL(sensor, setup()).WillOnce(Return(true));

  EXPECT_TRUE(sensor.begin(GetParam(), &wire));
  EXPECT_EQ(sensor.wire_, &wire);
  EXPECT_EQ(sensor.address_, address);
}

TEST_P(Bme280TwoWireTestWithAddress, GetsAddress) {
  sensor.address_ = address;
  EXPECT_EQ(sensor.getAddress(), sensor.address_);
}

TEST_P(Bme280TwoWireTestWith8BitCommunication, Writes) {
  {
    InSequence s;
    EXPECT_CALL(wire, beginTransmission(sensor.address_));
    EXPECT_CALL(wire, write(registerAddress));
    EXPECT_CALL(wire, write(expectedValue));
    EXPECT_CALL(wire, endTransmission());
  }

  sensor.write8(registerAddress, expectedValue);
}

TEST_P(Bme280TwoWireTestWith8BitCommunication, Reads) {
  {
    InSequence s;
    EXPECT_CALL(wire, beginTransmission(sensor.address_));
    EXPECT_CALL(wire, write(registerAddress));
    EXPECT_CALL(wire, endTransmission());
    EXPECT_CALL(wire, requestFrom(sensor.address_, 1u));
    EXPECT_CALL(wire, read()).WillOnce(Return(expectedValue));
  }

  EXPECT_EQ(sensor.read8(registerAddress), expectedValue);
}

TEST_P(Bme280TwoWireTestWith16BitCommunication, Reads) {
  {
    InSequence s;
    EXPECT_CALL(wire, beginTransmission(sensor.address_));
    EXPECT_CALL(wire, write(registerAddress));
    EXPECT_CALL(wire, endTransmission());
    EXPECT_CALL(wire, requestFrom(sensor.address_, 2u));
    EXPECT_CALL(wire, read())
        .WillOnce(Return(communicationSnapshot[1]))
        .WillOnce(Return(communicationSnapshot[2]));
  }

  EXPECT_EQ(sensor.read16(registerAddress), expectedValue);
}

TEST_P(Bme280TwoWireTestWith24BitCommunication, Reads) {
  {
    InSequence s;
    EXPECT_CALL(wire, beginTransmission(sensor.address_));
    EXPECT_CALL(wire, write(registerAddress));
    EXPECT_CALL(wire, endTransmission());
    EXPECT_CALL(wire, requestFrom(sensor.address_, 3u));
    EXPECT_CALL(wire, read())
        .WillOnce(Return(communicationSnapshot[1]))
        .WillOnce(Return(communicationSnapshot[2]))
        .WillOnce(Return(communicationSnapshot[3]));
  }

  EXPECT_EQ(sensor.read24(registerAddress), expectedValue);
}

#define PARAM_ADDRESS \
  Values(Bme280TwoWireAddress::Primary, Bme280TwoWireAddress::Secondary)

INSTANTIATE_TEST_SUITE_P(Addresses, Bme280TwoWireTestWithAddress,
                         PARAM_ADDRESS);

INSTANTIATE_TEST_SUITE_P(P_8_BIT_MESSAGES,
                         Bme280TwoWireTestWith8BitCommunication,
                         Combine(PARAM_ADDRESS, PARAM_8_BIT_REGISTER,
                                 PARAM_8_BIT_COMMUNICATION_SNAPSHOT));

INSTANTIATE_TEST_SUITE_P(P_16_BIT_MESSAGES,
                         Bme280TwoWireTestWith16BitCommunication,
                         Combine(PARAM_ADDRESS, PARAM_16_BIT_REGISTER,
                                 PARAM_16_BIT_COMMUNICATION_SNAPSHOT));

INSTANTIATE_TEST_SUITE_P(P_24_BIT_MESSAGES,
                         Bme280TwoWireTestWith24BitCommunication,
                         Combine(PARAM_ADDRESS, PARAM_24_BIT_REGISTER,
                                 PARAM_24_BIT_COMMUNICATION_SNAPSHOT));
