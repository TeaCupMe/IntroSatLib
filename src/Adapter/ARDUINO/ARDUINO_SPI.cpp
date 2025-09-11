#include "Adapter/SPI.h"
#include "SPI.h"

#if defined(ARDUINO)

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::_transfer(const uint8_t* out, uint8_t* in, uint8_t len) {    
    _hspi->beginTransaction();
    for (int i = 0; i < len; i++) {
        in[i] = _hspi->transfer(out[i]);
    }
    _hspi->endTransaction();
    return ISL_StatusTypeDef::ISL_OK // always ok?    
}  

#endif /* ARDUINO */