#define ISL_INTERNAL

#include "Adapter/SPI.h"

#ifdef ISL_SPI_ENABLED

#include "Device/SPIDevice.h"

namespace IntroSatLib {

SPIDevice::SPIDevice(interfaces::SPI _spi): spi(_spi) { }

SPIDevice::SPIDevice(interfaces::SPI _spi, interfaces::GPIO _cs, bool _csActiveLow): spi(_spi), cs(_cs)
{
    useCs = true;
    csActiveLow = _csActiveLow;
}

ISL_StatusTypeDef SPIDevice::Init() {
	return ISL_StatusTypeDef::ISL_OK;
}

void SPIDevice::SetCS(interfaces::GPIO _cs, bool _csActiveLow) {
    cs = _cs;
    useCs = true;
    csActiveLow = _csActiveLow;
}

void SPIDevice::Select() {
    useCs ? csActiveLow ? cs.reset() : cs.set() : (void)0;
}

void SPIDevice::Deselect() {
    useCs ? csActiveLow ? cs.set() : cs.reset() : (void)0;
}

ISL_StatusTypeDef SPIDevice::TransmitSPI(uint8_t *out, uint8_t len)
{
    uint8_t* temp = new uint8_t[len];
    return spi.transfer(out, temp, len);
}
ISL_StatusTypeDef SPIDevice::TransmitByteSPI(uint8_t out)
{
    uint8_t temp;
    return spi.transfer(&out, &temp, 1);
}
ISL_StatusTypeDef SPIDevice::ReceiveSPI(uint8_t *in, uint8_t len)
{
    uint8_t* temp = new uint8_t[len];
    return spi.transfer(temp, in, len);
}
ISL_StatusTypeDef SPIDevice::TransmitReceiveSPI(uint8_t *out, uint8_t *in, uint8_t len)
{
    return spi.transfer(out, in, len);
};
}
#endif /* ISL_SPI_ENABLED */
