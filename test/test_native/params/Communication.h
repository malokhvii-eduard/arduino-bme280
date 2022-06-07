#ifndef ARDUINO_BME280_TEST_NATIVE_PARAMS_COMMUNICATION_H_
#define ARDUINO_BME280_TEST_NATIVE_PARAMS_COMMUNICATION_H_

#include <array>

/* Google Test */
#include <gtest/gtest.h>

/* Library */
#include <Bme280.h>

// First number is always expected value for read or write operation
using CommunicationSnapshot = std::array<long, 5>;

#define P_8_BIT_MESSAGES 8BitMessages
#define P_16_BIT_MESSAGES 16BitMessages
#define P_16_BIT_LITTLE_ENDIAN_MESSAGES 16BitLittleEndianMessages
#define P_24_BIT_MESSAGES 24BitMessages

#define PARAM_8_BIT_REGISTER                                 \
  ::testing::Values(::internal::Bme280RegisterAddressConfig, \
                    ::internal::Bme280RegisterAddressCtrlHum)
#define PARAM_8_BIT_COMMUNICATION_SNAPSHOT \
  ::testing::Values(CommunicationSnapshot{21}, CommunicationSnapshot{34})

#define PARAM_16_BIT_REGISTER                               \
  ::testing::Values(::internal::Bme280RegisterAddressDigT1, \
                    ::internal::Bme280RegisterAddressDigH1)
#define PARAM_16_BIT_COMMUNICATION_SNAPSHOT              \
  ::testing::Values(CommunicationSnapshot{3349, 13, 21}, \
                    CommunicationSnapshot{8759, 34, 55})

#define PARAM_16_BIT_LITTLE_ENDIAD_REGISTER                 \
  ::testing::Values(::internal::Bme280RegisterAddressDigT2, \
                    ::internal::Bme280RegisterAddressDigH2)
#define PARAM_16_BIT_LITTLE_ENDIAD_COMMUNICATION_SNAPSHOT \
  ::testing::Values(CommunicationSnapshot{3328, 13},      \
                    CommunicationSnapshot{8704, 34})

#define PARAM_24_BIT_REGISTER                                         \
  ::testing::Values(::internal::Bme280RegisterAddressTemperatureData, \
                    ::internal::Bme280RegisterAddressPressureData)
#define PARAM_24_BIT_COMMUNICATION_SNAPSHOT                 \
  ::testing::Values(CommunicationSnapshot{197896, 3, 5, 8}, \
                    CommunicationSnapshot{527637, 8, 13, 21})

#endif  // ARDUINO_BME280_TEST_NATIVE_PARAMS_COMMUNICATION_H_
