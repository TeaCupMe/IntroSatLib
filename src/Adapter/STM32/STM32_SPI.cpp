#define INTROSATLIB_INTERNAL
#include "Adapter/SPI.h"

#if defined(HAL_SPI_MODULE_ENABLED) && !defined(ARDUINO) // If HAL SPI is enabled and no Arduino framework

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::_transfer(const uint8_t* out, uint8_t* in, uint8_t len)
{
	return (ISL_StatusTypeDef) HAL_SPI_TransmitReceive(_hspi, (uint8_t*) out, in, len, 1000);
}

#endif /* defined(HAL_SPI_MODULE_ENABLED) && !defined(ARDUINO) */

