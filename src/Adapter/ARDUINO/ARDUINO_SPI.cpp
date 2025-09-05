#include "Adapter/SPI.h"
#include "SPI.h"

#if defined(ARDUINO)
IntroSatLib::interfaces::SPI::SPI(SPIClass& spi, uint8_t csPin): _spi(spi), _csPin(csPin)
{}

void IntroSatLib::interfaces::SPI::begin()
{
    _spi.begin();
}

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::transfer(const uint8_t* out, uint8_t* in, uint8_t len) {

}  

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::setCs(GPIO_HANDLE_TYPE* port, uint16_t pin) {
    
}

#endif /* ARDUINO */