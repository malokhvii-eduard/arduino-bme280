#include <mocks/SPI.h>

SPIClass SPI;

bool SPISettings::operator==(const SPISettings& other) const {
  return _clock == other._clock && _bitOrder == other._bitOrder &&
         _dataMode == other._dataMode;
}
