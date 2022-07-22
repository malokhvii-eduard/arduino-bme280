#include <bitset>

/* Google Test */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* Fixtures */
#include <fixtures/AbstractBme280Test.h>

using ::testing::_;
using ::testing::Combine;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Values;

bool operator==(const Bme280CalibrationData& l,
                const Bme280CalibrationData& r) {
  return l.digT1 == r.digT1 && l.digT2 == r.digT2 && l.digT3 == r.digT3 &&
         l.digP1 == r.digP1 && l.digP2 == r.digP2 && l.digP3 == r.digP3 &&
         l.digP4 == r.digP4 && l.digP5 == r.digP5 && l.digP6 == r.digP6 &&
         l.digP7 == r.digP7 && l.digP8 == r.digP8 && l.digP9 == r.digP9 &&
         l.digH1 == r.digH1 && l.digH2 == r.digH2 && l.digH3 == r.digH3 &&
         l.digH4 == r.digH4 && l.digH5 == r.digH5 && l.digH6 == r.digH6;
}

bool operator==(const Bme280Settings& l, const Bme280Settings& r) {
  return l.mode == r.mode &&
         l.temperatureOversampling == r.temperatureOversampling &&
         l.pressureOversampling == r.pressureOversampling &&
         l.humidityOversampling == r.humidityOversampling &&
         l.filter == r.filter && l.standbyTime == r.standbyTime;
}

const Bme280CalibrationData expectedCalibrationData = {.digT1 = 28780,
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

TEST_F(AbstractBme280TestAlt, Setups) {
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressChipId));
  EXPECT_CALL(sensor, reset());

  EXPECT_CALL(*arduino, millis())
      .WillOnce(Return(0))
      .WillOnce(Return(1000))
      .WillOnce(Return(2000));
  EXPECT_CALL(sensor, isImUpdate())
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*arduino, delay(_)).Times(3);

  EXPECT_CALL(sensor, readCalibrationData());
  EXPECT_CALL(sensor, setSettings(Bme280Settings::defaults()));

  EXPECT_TRUE(sensor.setup());
  EXPECT_EQ(sensor.chipId_, 0);
}

TEST_F(AbstractBme280TestAlt, SetupsWhenTimeout) {
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressChipId));
  EXPECT_CALL(sensor, reset());

  EXPECT_CALL(*arduino, millis())
      .WillOnce(Return(0))
      .WillOnce(Return(1000))
      .WillOnce(Return(5000))
      .WillOnce(Return(15000));
  EXPECT_CALL(sensor, isImUpdate()).WillRepeatedly(Return(true));
  EXPECT_CALL(*arduino, delay(_)).Times(3);

  EXPECT_FALSE(sensor.setup());
}

TEST_P(AbstractBme280TestWithChipId, GetsChipId) {
  EXPECT_EQ(sensor.getChipId(), sensor.chipId_);
}

TEST_F(AbstractBme280Test, GetsCalibrationData) {
  EXPECT_EQ(sensor.getCalibrationData(), expectedCalibrationData);
}

TEST_P(AbstractBme280TestWithSettings, GetsSettings) {
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressConfig))
      .WillOnce(Return(settingsSnapshot[0]));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressCtrlHum))
      .WillOnce(Return(settingsSnapshot[1]));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressCtrlMeas))
      .WillOnce(Return(settingsSnapshot[2]));

  EXPECT_EQ(sensor.getSettings(), expectedSettings);
}

TEST_F(AbstractBme280Test, GetsTemperature) {
  EXPECT_CALL(sensor, read24(::internal::Bme280RegisterAddressTemperatureData))
      .WillOnce(Return(8408304));

  EXPECT_FLOAT_EQ(sensor.getTemperature(), 20.56);
}

TEST_F(AbstractBme280Test, GetsTemperatureWhenOversamplingOff) {
  EXPECT_CALL(sensor, read24(::internal::Bme280RegisterAddressTemperatureData))
      .WillOnce(Return(0x800000));

  EXPECT_TRUE(isnan(sensor.getTemperature()));
}

TEST_F(AbstractBme280Test, GetsPressure) {
  {
    InSequence s;
    EXPECT_CALL(sensor,
                read24(::internal::Bme280RegisterAddressTemperatureData))
        .WillOnce(Return(8408304));
    EXPECT_CALL(sensor, read24(::internal::Bme280RegisterAddressPressureData))
        .WillOnce(Return(6029792));
  }

  EXPECT_FLOAT_EQ(sensor.getPressure(), 100958.73);
}

TEST_F(AbstractBme280Test, GetsPressureWhenZeroDivision) {
  {
    InSequence s;
    EXPECT_CALL(sensor,
                read24(::internal::Bme280RegisterAddressTemperatureData))
        .WillOnce(Return(8408304));
    EXPECT_CALL(sensor, read24(::internal::Bme280RegisterAddressPressureData))
        .WillOnce(Return(6029792));
  }

  sensor.calibrationData_.digP1 = 0;  // Force zero division
  EXPECT_FLOAT_EQ(sensor.getPressure(), 0);
}

TEST_F(AbstractBme280Test, GetsPressureWhenOversamplingOff) {
  {
    InSequence s;
    EXPECT_CALL(sensor,
                read24(::internal::Bme280RegisterAddressTemperatureData));
    EXPECT_CALL(sensor, read24(::internal::Bme280RegisterAddressPressureData))
        .WillOnce(Return(0x800000));
  }

  EXPECT_TRUE(isnan(sensor.getPressure()));
}

TEST_F(AbstractBme280Test, GetsHumidity) {
  {
    InSequence s;
    EXPECT_CALL(sensor,
                read24(::internal::Bme280RegisterAddressTemperatureData))
        .WillOnce(Return(8408304));
    EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressHumidityData))
        .WillOnce(Return(29384));
  }

  EXPECT_NEAR(sensor.getHumidity(), 44.13, 0.1);
}

TEST_F(AbstractBme280Test, GetsHumidityWhenOversamplingOff) {
  {
    InSequence s;
    EXPECT_CALL(sensor,
                read24(::internal::Bme280RegisterAddressTemperatureData));
    EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressHumidityData))
        .WillOnce(Return(0x8000));
  }

  EXPECT_TRUE(isnan(sensor.getHumidity()));
}

TEST_F(AbstractBme280Test, GetsIsImUpdate) {
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressStatus))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_FALSE(sensor.isImUpdate());
  EXPECT_TRUE(sensor.isImUpdate());
}

TEST_F(AbstractBme280Test, GetsIsImMeasuring) {
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressStatus))
      .WillOnce(Return(0))
      .WillOnce(Return(0b1000));

  EXPECT_FALSE(sensor.isMeasuring());
  EXPECT_TRUE(sensor.isMeasuring());
}

TEST_P(AbstractBme280TestWithSettings, SetsSettings) {
  {
    InSequence s;
    EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlMeas,
                               static_cast<uint8_t>(Bme280Mode::Sleep)));
    EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressConfig,
                               settingsSnapshot[0]));
    EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlHum,
                               settingsSnapshot[1]));
    EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlMeas,
                               settingsSnapshot[2]));
    EXPECT_CALL(*arduino, delay(_));
  }

  sensor.setSettings(expectedSettings);
};

TEST_F(AbstractBme280Test, Resets) {
  EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressReset, 0xB6));
  sensor.reset();
}

TEST_F(AbstractBme280Test, Sleeps) {
  ON_CALL(sensor, read8(::internal::Bme280RegisterAddressCtrlMeas))
      .WillByDefault(Return(static_cast<uint8_t>(Bme280Mode::Normal)));

  EXPECT_CALL(sensor, write8(_, _)).Times(2);
  EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlMeas,
                             static_cast<uint8_t>(Bme280Mode::Sleep)))
      .Times(2);
  EXPECT_CALL(*arduino, delay(_));

  sensor.sleep();
}

TEST_F(AbstractBme280Test, WakesUp) {
  ON_CALL(sensor, read8(::internal::Bme280RegisterAddressCtrlMeas))
      .WillByDefault(Return(static_cast<uint8_t>(Bme280Mode::Sleep)));

  EXPECT_CALL(sensor, write8(_, _)).Times(3);
  EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlMeas,
                             static_cast<uint8_t>(Bme280Mode::Normal)));
  EXPECT_CALL(*arduino, delay(_));

  sensor.wakeUp();
}

TEST_F(AbstractBme280Test, WakesUpForced) {
  ON_CALL(sensor, read8(::internal::Bme280RegisterAddressCtrlMeas))
      .WillByDefault(Return(static_cast<uint8_t>(Bme280Mode::Sleep)));

  EXPECT_CALL(sensor, write8(_, _)).Times(3);
  EXPECT_CALL(sensor, write8(::internal::Bme280RegisterAddressCtrlMeas,
                             static_cast<uint8_t>(Bme280Mode::Forced)));
  EXPECT_CALL(*arduino, delay(_));

  sensor.wakeUpForced();
}

TEST_F(AbstractBme280Test, ReadsCalibrationData) {
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigT1))
      .WillRepeatedly(Return(27760));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigT2))
      .WillRepeatedly(Return(37223));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigT3))
      .WillRepeatedly(Return(12800));

  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP1))
      .WillRepeatedly(Return(19595));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP2))
      .WillRepeatedly(Return(52950));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP3))
      .WillRepeatedly(Return(53259));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP4))
      .WillRepeatedly(Return(17430));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP5))
      .WillRepeatedly(Return(3584));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP6))
      .WillRepeatedly(Return(63999));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP7))
      .WillRepeatedly(Return(44070));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP8))
      .WillRepeatedly(Return(2776));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigP9))
      .WillRepeatedly(Return(48400));

  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH1))
      .WillRepeatedly(Return(75));
  EXPECT_CALL(sensor, read16(::internal::Bme280RegisterAddressDigH2))
      .WillRepeatedly(Return(25857));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH3))
      .WillRepeatedly(Return(0));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH4))
      .WillRepeatedly(Return(20));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH5))
      .WillRepeatedly(Return(44));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH5 + 1))
      .WillRepeatedly(Return(3));
  EXPECT_CALL(sensor, read8(::internal::Bme280RegisterAddressDigH6))
      .WillRepeatedly(Return(30));

  sensor.calibrationData_ = {};
  sensor.readCalibrationData();
  EXPECT_EQ(sensor.calibrationData_, expectedCalibrationData);
}

TEST_P(AbstractBme280TestWith16BitCommunication, ReadsSigned) {
  EXPECT_CALL(sensor, read16(registerAddress))
      .WillOnce(Return(communicationSnapshot[0]));

  EXPECT_EQ(sensor.read16Signed(registerAddress), expectedValue);
}

TEST_P(AbstractBme280TestWith16BitLittleEndianCommunication, Reads) {
  EXPECT_CALL(sensor, read16(registerAddress))
      .WillOnce(Return(communicationSnapshot[1]));

  EXPECT_EQ(sensor.read16LittleEndian(registerAddress), expectedValue);
}

TEST_P(AbstractBme280TestWith16BitLittleEndianCommunication, ReadsSigned) {
  EXPECT_CALL(sensor, read16(registerAddress))
      .WillOnce(Return(communicationSnapshot[1]));

  EXPECT_EQ(sensor.read16SignedLittleEndian(registerAddress), expectedValue);
}

INSTANTIATE_TEST_SUITE_P(ChipIds, AbstractBme280TestWithChipId, Values(13, 21));

INSTANTIATE_TEST_SUITE_P(Settings, AbstractBme280TestWithSettings,
                         Values(std::make_tuple(Bme280Settings::indoor(),
                                                SettingsSnapshot{176, 1, 87}),
                                std::make_tuple(Bme280Settings::gaming(),
                                                SettingsSnapshot{16, 0, 111})));

INSTANTIATE_TEST_SUITE_P(P_16_BIT_MESSAGES,
                         AbstractBme280TestWith16BitCommunication,
                         Combine(PARAM_16_BIT_REGISTER,
                                 PARAM_16_BIT_COMMUNICATION_SNAPSHOT));

INSTANTIATE_TEST_SUITE_P(
    P_16_BIT_LITTLE_ENDIAN_MESSAGES,
    AbstractBme280TestWith16BitLittleEndianCommunication,
    Combine(PARAM_16_BIT_LITTLE_ENDIAD_REGISTER,
            PARAM_16_BIT_LITTLE_ENDIAD_COMMUNICATION_SNAPSHOT));
