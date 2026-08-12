#define ISL_INTERNAL
#include "Adapter/SPI.h"

#if defined(ARDUINO)

#include "SPI.h"


ISL_StatusTypeDef IntroSatLib::interfaces::SPI::transfer(const uint8_t* out, uint8_t* in, uint8_t len) {    
    // _hspi->beginTransaction(); // TODO Надо это делать, но ему нужен аргумент SPISettings. 
    for (int i = 0; i < len; i++) {
        in[i] = _hspi->transfer(out[i]);
    }
    // _hspi->endTransaction();
    return ISL_StatusTypeDef::ISL_OK; // always ok?    
}

#endif /* ARDUINO */