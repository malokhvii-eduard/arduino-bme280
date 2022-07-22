#include <Bme280.h>
#include <assert.h>

#ifndef ARDUINO_BME280_UNIT_TEST
/* Arduino */
#include <Arduino.h>
#else
/* Mocks */
#include <mocks/Arduino.h>
#endif

Bme280::~Bme280() {}

namespace internal {

AbstractBme280::~AbstractBme280() {}

uint8_t AbstractBme280::getChipId() const { return chipId_; }
Bme280CalibrationData AbstractBme280::getCalibrationData() const {
  return calibrationData_;
}

Bme280Settings AbstractBme280::getSettings() const {
  config_.value = read8(Bme280RegisterAddressConfig);
  ctrlHum_.value = read8(Bme280RegisterAddressCtrlHum);
  ctrlMeas_.value = read8(Bme280RegisterAddressCtrlMeas);

  return {
      .mode = static_cast<Bme280Mode>(ctrlMeas_.mode),
      .temperatureOversampling =
          static_cast<Bme280Oversampling>(ctrlMeas_.osrsT),
      .pressureOversampling = static_cast<Bme280Oversampling>(ctrlMeas_.osrsP),
      .humidityOversampling = static_cast<Bme280Oversampling>(ctrlHum_.osrsH),
      .filter = static_cast<Bme280Filter>(config_.filter),
      .standbyTime = static_cast<Bme280StandbyTime>(config_.tSb)};
}

float AbstractBme280::getTemperature() const {
  int32_t rawTemperature = read24(Bme280RegisterAddressTemperatureData);
  if (rawTemperature == 0x800000) {
    return NAN;
  }
  rawTemperature >>= 4;

  int32_t var1 =
      ((((rawTemperature >> 3) - ((int32_t)calibrationData_.digT1 << 1))) *
       ((int32_t)calibrationData_.digT2)) >>
      11;

  int32_t var2 =
      (((((rawTemperature >> 4) - ((int32_t)calibrationData_.digT1)) *
         ((rawTemperature >> 4) - ((int32_t)calibrationData_.digT1))) >>
        12) *
       ((int32_t)calibrationData_.digT3)) >>
      14;

  temperatureFine_ = var1 + var2;

  float temperature = (temperatureFine_ * 5 + 128) >> 8;
  return temperature / 100;
}

float AbstractBme280::getPressure() const {
  getTemperature();

  int32_t rawPressure = read24(Bme280RegisterAddressPressureData);
  if (rawPressure == 0x800000) {
    return NAN;
  }
  rawPressure >>= 4;

  int64_t var1 = ((int64_t)temperatureFine_) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)calibrationData_.digP6;
  var2 = var2 + ((var1 * (int64_t)calibrationData_.digP5) << 17);
  var2 = var2 + (((int64_t)calibrationData_.digP4) << 35);
  var1 = ((var1 * var1 * (int64_t)calibrationData_.digP3) >> 8) +
         ((var1 * (int64_t)calibrationData_.digP2) << 12);
  var1 =
      (((((int64_t)1) << 47) + var1)) * ((int64_t)calibrationData_.digP1) >> 33;

  if (var1 == 0) {
    return 0;
  }

  int64_t pressure = 1048576 - rawPressure;
  pressure = (((pressure << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)calibrationData_.digP9) * (pressure >> 13) *
          (pressure >> 13)) >>
         25;
  var2 = (((int64_t)calibrationData_.digP8) * pressure) >> 19;

  pressure = ((pressure + var1 + var2) >> 8) +
             (((int64_t)calibrationData_.digP7) << 4);
  return (float)pressure / 256;
}

float AbstractBme280::getHumidity() const {
  getTemperature();

  int32_t rawHumidity = read16(Bme280RegisterAddressHumidityData);
  if (rawHumidity == 0x8000) {
    return NAN;
  }

  int32_t var1 = (temperatureFine_ - ((int32_t)76800));
  var1 = (((((rawHumidity << 14) - (((int32_t)calibrationData_.digH4) << 20) -
             (((int32_t)calibrationData_.digH5) * var1)) +
            ((int32_t)16384)) >>
           15) *
          (((((((var1 * ((int32_t)calibrationData_.digH6)) >> 10) *
               (((var1 * ((int32_t)calibrationData_.digH3)) >> 11) +
                ((int32_t)32768))) >>
              10) +
             ((int32_t)2097152)) *
                ((int32_t)calibrationData_.digH2) +
            8192) >>
           14));

  var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) *
                   ((int32_t)calibrationData_.digH1)) >>
                  4));
  var1 = (var1 < 0) ? 0 : var1;
  var1 = (var1 > 419430400) ? 419430400 : var1;

  float humidity = (var1 >> 12);
  return humidity / 1024;
}

void AbstractBme280::setSettings(const Bme280Settings settings) {
  ctrlMeas_.mode = static_cast<uint8_t>(settings.mode);
  ctrlMeas_.osrsT = static_cast<uint8_t>(settings.temperatureOversampling);
  ctrlMeas_.osrsP = static_cast<uint8_t>(settings.pressureOversampling);

  ctrlHum_.osrsH = static_cast<uint8_t>(settings.humidityOversampling);

  config_.filter = static_cast<uint8_t>(settings.filter);
  config_.tSb = static_cast<uint8_t>(settings.standbyTime);

  write8(Bme280RegisterAddressCtrlMeas,
         static_cast<uint8_t>(Bme280Mode::Sleep));

  write8(Bme280RegisterAddressConfig, config_.value);
  write8(Bme280RegisterAddressCtrlHum, ctrlHum_.value);
  write8(Bme280RegisterAddressCtrlMeas, ctrlMeas_.value);

  delay(100);
}

void AbstractBme280::reset() { write8(Bme280RegisterAddressReset, 0xB6); }

void AbstractBme280::sleep() {
  auto settings = getSettings();
  settings.mode = Bme280Mode::Sleep;
  setSettings(settings);
}

void AbstractBme280::wakeUp() {
  auto settings = getSettings();
  settings.mode = Bme280Mode::Normal;
  setSettings(settings);
}

void AbstractBme280::wakeUpForced() {
  auto settings = getSettings();
  settings.mode = Bme280Mode::Forced;
  setSettings(settings);
}

bool AbstractBme280::setup() {
  chipId_ = read8(Bme280RegisterAddressChipId);

  reset();
  delay(10);

  const uint32_t startTime = millis();
  while (isImUpdate()) {
    if (millis() - startTime >= ARDUINO_BME280_TIMEOUT) {
      return false;
    }

    delay(20);
  }

  readCalibrationData();
  setSettings(Bme280Settings::defaults());

  return true;
}

bool AbstractBme280::isImUpdate() const {
  Bme280RegisterStatus status;
  status.value = read8(Bme280RegisterAddressStatus);
  return status.imUpdate != 0;
}

bool AbstractBme280::isMeasuring() const {
  Bme280RegisterStatus status;
  status.value = read8(Bme280RegisterAddressStatus);
  return status.measuring == 1;
}

void AbstractBme280::readCalibrationData() const {
  calibrationData_.digT1 = read16LittleEndian(Bme280RegisterAddressDigT1);
  calibrationData_.digT2 = read16SignedLittleEndian(Bme280RegisterAddressDigT2);
  calibrationData_.digT3 = read16SignedLittleEndian(Bme280RegisterAddressDigT3);

  calibrationData_.digP1 = read16LittleEndian(Bme280RegisterAddressDigP1);
  calibrationData_.digP2 = read16SignedLittleEndian(Bme280RegisterAddressDigP2);
  calibrationData_.digP3 = read16SignedLittleEndian(Bme280RegisterAddressDigP3);
  calibrationData_.digP4 = read16SignedLittleEndian(Bme280RegisterAddressDigP4);
  calibrationData_.digP5 = read16SignedLittleEndian(Bme280RegisterAddressDigP5);
  calibrationData_.digP6 = read16SignedLittleEndian(Bme280RegisterAddressDigP6);
  calibrationData_.digP7 = read16SignedLittleEndian(Bme280RegisterAddressDigP7);
  calibrationData_.digP8 = read16SignedLittleEndian(Bme280RegisterAddressDigP8);
  calibrationData_.digP9 = read16SignedLittleEndian(Bme280RegisterAddressDigP9);

  calibrationData_.digH1 = read8(Bme280RegisterAddressDigH1);
  calibrationData_.digH2 = read16SignedLittleEndian(Bme280RegisterAddressDigH2);
  calibrationData_.digH3 = read8(Bme280RegisterAddressDigH3);
  calibrationData_.digH4 =
      (static_cast<int8_t>(read8(Bme280RegisterAddressDigH4)) << 4) |
      (read8(Bme280RegisterAddressDigH4 + 1) & 0xF);
  calibrationData_.digH5 =
      (static_cast<int8_t>(read8(Bme280RegisterAddressDigH5 + 1)) << 4) |
      (read8(Bme280RegisterAddressDigH5) >> 4);
  calibrationData_.digH6 =
      static_cast<int8_t>(read8(Bme280RegisterAddressDigH6));
}

int16_t AbstractBme280::read16Signed(const uint8_t registerAddress) const {
  return static_cast<int16_t>(read16(registerAddress));
}

uint16_t AbstractBme280::read16LittleEndian(
    const uint8_t registerAddress) const {
  uint16_t value = read16(registerAddress);
  return (value >> 8) | (value << 8);
}

int16_t AbstractBme280::read16SignedLittleEndian(
    const uint8_t registerAddress) const {
  return static_cast<int16_t>(read16LittleEndian(registerAddress));
}

}  // namespace internal

Bme280TwoWire::Bme280TwoWire() : wire_(nullptr) {}

Bme280TwoWire::~Bme280TwoWire() {}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)

bool Bme280TwoWire::begin() { return begin(Bme280TwoWireAddress::Primary); }

bool Bme280TwoWire::begin(const Bme280TwoWireAddress address) {
  return begin(address, &Wire);
}

#endif

bool Bme280TwoWire::begin(const Bme280TwoWireAddress address, TwoWire *wire) {
  address_ = static_cast<uint8_t>(address);
  wire_ = wire;
  return setup();
}

uint8_t Bme280TwoWire::getAddress() const { return address_; }

void Bme280TwoWire::write8(const uint8_t registerAddress, uint8_t value) {
  assert(wire_ != nullptr);

  wire_->beginTransmission(address_);
  wire_->write(registerAddress);
  wire_->write(value);
  wire_->endTransmission();
}

uint8_t Bme280TwoWire::read8(const uint8_t registerAddress) const {
  assert(wire_ != nullptr);

  wire_->beginTransmission(address_);
  wire_->write(registerAddress);
  wire_->endTransmission();
  wire_->requestFrom(address_, 1u);

  return wire_->read();
}

uint16_t Bme280TwoWire::read16(const uint8_t registerAddress) const {
  assert(wire_ != nullptr);

  wire_->beginTransmission(address_);
  wire_->write(registerAddress);
  wire_->endTransmission();
  wire_->requestFrom(address_, 2u);

  return (wire_->read() << 8) | wire_->read();
}

uint32_t Bme280TwoWire::read24(const uint8_t registerAddress) const {
  assert(wire_ != nullptr);

  wire_->beginTransmission(address_);
  wire_->write(registerAddress);
  wire_->endTransmission();
  wire_->requestFrom(address_, 3u);

  uint32_t value = wire_->read();
  value <<= 8;
  value |= wire_->read();
  value <<= 8;
  value |= wire_->read();

  return value;
}

Bme280FourWire::Bme280FourWire() : spi_(nullptr) {}

Bme280FourWire::~Bme280FourWire() {}

const SPISettings Bme280FourWire::spiSettings_ =
    SPISettings(SPISettings(500000, MSBFIRST, SPI_MODE0));

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SPI)

bool Bme280FourWire::begin(const uint8_t csPin) { return begin(csPin, &SPI); }

#endif

bool Bme280FourWire::begin(const uint8_t csPin, SPIClass *spi) {
  spi_ = spi;
  csPin_ = csPin;

  pinMode(csPin_, OUTPUT);
  digitalWrite(csPin_, HIGH);

  return setup();
}

uint8_t Bme280FourWire::getCsPin() const { return csPin_; }

void Bme280FourWire::write8(const uint8_t registerAddress, uint8_t value) {
  assert(spi_ != nullptr);

  ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 0;

  spi_->beginTransaction(spiSettings_);
  digitalWrite(csPin_, LOW);

  spi_->transfer(control.value);
  spi_->transfer(value);

  digitalWrite(csPin_, HIGH);
  spi_->endTransaction();
}

uint8_t Bme280FourWire::read8(const uint8_t registerAddress) const {
  assert(spi_ != nullptr);

  ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  spi_->beginTransaction(spiSettings_);
  digitalWrite(csPin_, LOW);

  spi_->transfer(control.value);
  uint8_t value = spi_->transfer(0);

  digitalWrite(csPin_, HIGH);
  spi_->endTransaction();

  return value;
}

uint16_t Bme280FourWire::read16(const uint8_t registerAddress) const {
  assert(spi_ != nullptr);

  ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  spi_->beginTransaction(spiSettings_);
  digitalWrite(csPin_, LOW);

  spi_->transfer(control.value);
  uint16_t value = (spi_->transfer(0) << 8) | spi_->transfer(0);

  digitalWrite(csPin_, HIGH);
  spi_->endTransaction();

  return value;
}

uint32_t Bme280FourWire::read24(const uint8_t registerAddress) const {
  assert(spi_ != nullptr);

  ControlByte control;
  control.registerAddress = registerAddress;
  control.rw = 1;

  spi_->beginTransaction(spiSettings_);
  digitalWrite(csPin_, LOW);

  spi_->transfer(control.value);
  uint32_t value = spi_->transfer(0);
  value <<= 8;
  value |= spi_->transfer(0);
  value <<= 8;
  value |= spi_->transfer(0);

  digitalWrite(csPin_, HIGH);
  spi_->endTransaction();

  return value;
}
