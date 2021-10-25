#include <Arduino.h>
#include <Bme280.h>

Bme280TwoWire sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);

  Serial.println();

  sensor.begin(Bme280TwoWireAddress::Primary);

  sensor.setSettings(Bme280Settings::defaults());
  sensor.setSettings(Bme280Settings::indoor());
  sensor.setSettings(Bme280Settings::weatherMonitoring());
  sensor.setSettings(Bme280Settings::gaming());
  sensor.setSettings(Bme280Settings::humiditySensing());
}

void loop() {}
