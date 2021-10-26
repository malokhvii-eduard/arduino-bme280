#include <Arduino.h>
#include <Bme280.h>

Bme280FourWire sensor;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  sensor.begin(D1, &SPI);
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
