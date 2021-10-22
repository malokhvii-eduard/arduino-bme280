#include <mocks/Wire.h>

TwoWireMock Wire;

TwoWire::~TwoWire() {}

void TwoWire::beginTransmission(uint8_t) {}

uint8_t TwoWire::endTransmission() { return 0; }

uint8_t TwoWire::requestFrom(uint8_t, uint8_t) { return 0; }

size_t TwoWire::write(uint8_t) { return 0; }

int TwoWire::read() { return 0; }
