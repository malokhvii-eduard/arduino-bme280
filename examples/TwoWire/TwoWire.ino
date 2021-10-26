#include <Arduino.h>
#include <Bme280.h>

Bme280TwoWire sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);

  Serial.println();

  sensor.begin(Bme280TwoWireAddress::Primary);
  sensor.setSettings(Bme280Settings::indoor());
}

void loop() {
  const auto temperature = String(sensor.getTemperature()) + " °C";
  const auto pressure = String(sensor.getPressure() / 100.0) + " hPa";
  const auto humidity = String(sensor.getHumidity()) + " %";

  String measurements = temperature + ", " + pressure + ", " + humidity;
  Serial.println(measurements);

  delay(10000);
}
