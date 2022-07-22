#ifndef ARDUINO_BME280_INCLUDE_BME280_H_
#define ARDUINO_BME280_INCLUDE_BME280_H_

#include <stdint.h>

#ifndef ARDUINO_BME280_UNIT_TEST
/* Arduino */
#include <SPI.h>
#include <Wire.h>

#define __NOT_VIRTUAL
#else
/* Mocks */
#include <mocks/SPI.h>
#include <mocks/Wire.h>

#define __NOT_VIRTUAL virtual

#define protected public
#define private public
#define final
#endif

#define ARDUINO_BME280_TIMEOUT 10000  // 10 seconds

enum class Bme280Mode : uint8_t { Sleep = 0b00, Forced = 0b01, Normal = 0b11 };

enum class Bme280Oversampling : uint8_t {
  Off = 0b000,
  X1 = 0b001,
  X2 = 0b010,
  X4 = 0b011,
  X8 = 0b100,
  X16 = 0b101
};

enum class Bme280Filter : uint8_t {
  Off = 0b000,
  X2 = 0b001,
  X4 = 0b010,
  X8 = 0b011,
  X16 = 0b100
};

enum class Bme280StandbyTime : uint8_t {
  Ms0_5 = 0b000,
  Ms10 = 0b110,
  Ms20 = 0b111,
  Ms62_5 = 0b001,
  Ms125 = 0b010,
  Ms250 = 0b011,
  Ms500 = 0b100,
  Ms1000 = 0b101
};

struct Bme280CalibrationData {
  uint16_t digT1;  // Temperature compensation value
  int16_t digT2;   // Temperature compensation value
  int16_t digT3;   // Temperature compensation value
  uint16_t digP1;  // Pressure compensation value
  int16_t digP2;   // Pressure compensation value
  int16_t digP3;   // Pressure compensation value
  int16_t digP4;   // Pressure compensation value
  int16_t digP5;   // Pressure compensation value
  int16_t digP6;   // Pressure compensation value
  int16_t digP7;   // Pressure compensation value
  int16_t digP8;   // Pressure compensation value
  int16_t digP9;   // Pressure compensation value
  uint8_t digH1;   // Humidity compensation value
  int16_t digH2;   // Humidity compensation value
  uint8_t digH3;   // Humidity compensation value
  int16_t digH4;   // Humidity compensation value
  int16_t digH5;   // Humidity compensation value
  int8_t digH6;    // Humidity compensation value
};

struct Bme280Settings {
  static Bme280Settings defaults() {
    return {.mode = Bme280Mode::Normal,
            .temperatureOversampling = Bme280Oversampling::X16,
            .pressureOversampling = Bme280Oversampling::X16,
            .humidityOversampling = Bme280Oversampling::X16,
            .filter = Bme280Filter::Off,
            .standbyTime = Bme280StandbyTime::Ms0_5};
  }

  static Bme280Settings weatherMonitoring() {
    return {.mode = Bme280Mode::Forced,
            .temperatureOversampling = Bme280Oversampling::X1,
            .pressureOversampling = Bme280Oversampling::X1,
            .humidityOversampling = Bme280Oversampling::X1,
            .filter = Bme280Filter::Off,
            .standbyTime = Bme280StandbyTime::Ms10};
  }

  static Bme280Settings humiditySensing() {
    return {.mode = Bme280Mode::Forced,
            .temperatureOversampling = Bme280Oversampling::X1,
            .pressureOversampling = Bme280Oversampling::Off,
            .humidityOversampling = Bme280Oversampling::X1,
            .filter = Bme280Filter::Off,
            .standbyTime = Bme280StandbyTime::Ms10};
  }

  static Bme280Settings indoor() {
    return {.mode = Bme280Mode::Normal,
            .temperatureOversampling = Bme280Oversampling::X2,
            .pressureOversampling = Bme280Oversampling::X16,
            .humidityOversampling = Bme280Oversampling::X1,
            .filter = Bme280Filter::X16,
            .standbyTime = Bme280StandbyTime::Ms1000};
  }

  static Bme280Settings gaming() {
    return {.mode = Bme280Mode::Normal,
            .temperatureOversampling = Bme280Oversampling::X4,
            .pressureOversampling = Bme280Oversampling::X4,
            .humidityOversampling = Bme280Oversampling::Off,
            .filter = Bme280Filter::X16,
            .standbyTime = Bme280StandbyTime::Ms0_5};
  }

  Bme280Mode mode;
  Bme280Oversampling temperatureOversampling;
  Bme280Oversampling pressureOversampling;
  Bme280Oversampling humidityOversampling;
  Bme280Filter filter;
  Bme280StandbyTime standbyTime;
};

class Bme280 {
 public:
  virtual ~Bme280();

  virtual uint8_t getChipId() const = 0;
  virtual Bme280CalibrationData getCalibrationData() const = 0;
  virtual Bme280Settings getSettings() const = 0;

  virtual float getTemperature() const = 0;
  virtual float getPressure() const = 0;
  virtual float getHumidity() const = 0;

  virtual void setSettings(const Bme280Settings settings) = 0;
  virtual void reset() = 0;

  virtual void sleep() = 0;
  virtual void wakeUp() = 0;
  virtual void wakeUpForced() = 0;
};

namespace internal {

enum Bme280RegisterAddress : uint8_t {
  Bme280RegisterAddressDigT1 = 0x88,
  Bme280RegisterAddressDigT2 = 0x8A,
  Bme280RegisterAddressDigT3 = 0x8C,
  Bme280RegisterAddressDigP1 = 0x8E,
  Bme280RegisterAddressDigP2 = 0x90,
  Bme280RegisterAddressDigP3 = 0x92,
  Bme280RegisterAddressDigP4 = 0x94,
  Bme280RegisterAddressDigP5 = 0x96,
  Bme280RegisterAddressDigP6 = 0x98,
  Bme280RegisterAddressDigP7 = 0x9A,
  Bme280RegisterAddressDigP8 = 0x9C,
  Bme280RegisterAddressDigP9 = 0x9E,
  Bme280RegisterAddressDigH1 = 0xA1,
  Bme280RegisterAddressDigH2 = 0xE1,
  Bme280RegisterAddressDigH3 = 0xE3,
  Bme280RegisterAddressDigH4 = 0xE4,
  Bme280RegisterAddressDigH5 = 0xE5,
  Bme280RegisterAddressDigH6 = 0xE7,
  Bme280RegisterAddressChipId = 0xD0,
  Bme280RegisterAddressVersion = 0xD1,
  Bme280RegisterAddressReset = 0xE0,
  Bme280RegisterAddressCalibData = 0xE1,
  Bme280RegisterAddressCtrlHum = 0xF2,
  Bme280RegisterAddressStatus = 0xF3,
  Bme280RegisterAddressCtrlMeas = 0xF4,
  Bme280RegisterAddressConfig = 0xF5,
  Bme280RegisterAddressPressureData = 0xF7,
  Bme280RegisterAddressTemperatureData = 0xFA,
  Bme280RegisterAddressHumidityData = 0xFD
};

union Bme280RegisterStatus {
  struct {
    uint8_t imUpdate : 1;
    uint8_t none1 : 2;
    uint8_t measuring : 1;
    uint8_t none2 : 4;
  };
  uint8_t value;
};

union Bme280RegisterConfig {
  struct {
    uint8_t spi3wEn : 1;
    uint8_t mone : 1;
    uint8_t filter : 3;
    uint8_t tSb : 3;
  };
  uint8_t value;
};

union Bme280RegisterCtrlHum {
  struct {
    uint8_t osrsH : 3;
    uint8_t none : 5;
  };
  uint8_t value;
};

union Bme280RegisterCtrlMeas {
  struct {
    uint8_t mode : 2;
    uint8_t osrsP : 3;
    uint8_t osrsT : 3;
  };
  uint8_t value;
};

class AbstractBme280 : public Bme280 {
 public:
  virtual ~AbstractBme280();

  uint8_t getChipId() const final;
  Bme280CalibrationData getCalibrationData() const final;
  Bme280Settings getSettings() const final;

  float getTemperature() const final;
  float getPressure() const final;
  float getHumidity() const final;

  void setSettings(const Bme280Settings settings) final;
  void reset() final;

  void sleep() final;
  void wakeUp() final;
  void wakeUpForced() final;

 protected:
  __NOT_VIRTUAL bool setup();

 private:
  bool isMeasuring() const;

  __NOT_VIRTUAL bool isImUpdate() const;
  __NOT_VIRTUAL void readCalibrationData() const;

  virtual void write8(const uint8_t registerAddress, uint8_t value) = 0;
  virtual uint8_t read8(const uint8_t registerAddress) const = 0;
  virtual uint16_t read16(const uint8_t registerAddress) const = 0;
  int16_t read16Signed(const uint8_t registerAddress) const;
  uint16_t read16LittleEndian(const uint8_t registerAddress) const;
  int16_t read16SignedLittleEndian(const uint8_t registerAddress) const;
  virtual uint32_t read24(const uint8_t registerAddress) const = 0;

  uint8_t chipId_;

  mutable Bme280CalibrationData calibrationData_;
  mutable int32_t temperatureFine_;

  mutable Bme280RegisterConfig config_;
  mutable Bme280RegisterCtrlHum ctrlHum_;
  mutable Bme280RegisterCtrlMeas ctrlMeas_;
};

};  // namespace internal

enum class Bme280TwoWireAddress : uint8_t { Primary = 0x76, Secondary = 0x77 };

class Bme280TwoWire final : public ::internal::AbstractBme280 {
 public:
  Bme280TwoWire();
  virtual ~Bme280TwoWire();

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
  bool begin();
  bool begin(const Bme280TwoWireAddress address);
#endif
  bool begin(const Bme280TwoWireAddress address, TwoWire *wire);

  uint8_t getAddress() const;

 private:
  void write8(const uint8_t registerAddress, uint8_t value);
  uint8_t read8(const uint8_t registerAddress) const;
  uint16_t read16(const uint8_t registerAddress) const;
  uint32_t read24(const uint8_t registerAddress) const;

  uint8_t address_;
  TwoWire *wire_;
};

class Bme280FourWire final : public ::internal::AbstractBme280 {
 public:
  Bme280FourWire();
  virtual ~Bme280FourWire();

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SPI)
  bool begin(const uint8_t csPin);
#endif
  bool begin(const uint8_t csPin, SPIClass *spi);

  uint8_t getCsPin() const;

 private:
  union ControlByte {
    struct {
      uint8_t registerAddress : 7;
      uint8_t rw : 1;
    };
    uint8_t value;
  };

  void write8(const uint8_t registerAddress, uint8_t value);
  uint8_t read8(const uint8_t registerAddress) const;
  uint16_t read16(const uint8_t registerAddress) const;
  uint32_t read24(const uint8_t registerAddress) const;

  uint8_t csPin_;
  SPIClass *spi_;
  static const SPISettings spiSettings_;
};

#undef __NOT_VIRTUAL

#ifdef ARDUINO_BME280_UNIT_TEST
#undef protected
#undef private
#undef final
#endif

#endif  // ARDUINO_BME280_INCLUDE_BME280_H_
