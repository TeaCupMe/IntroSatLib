
#define ISL_INTERNAL

#include "Adapter/I2C.h"
#include "Adapter/GPIO.h"
#include "Adapter/System.h"

#if defined(ISL_I2C_ENABLED) and defined(ISL_GPIO_ENABLED)

#include <Device/M24C02/M24C02.h>

namespace IntroSatLib {

ISL_StatusTypeDef M24C02::Read(uint32_t addr, uint8_t *data, uint16_t len)
{
	if (addr + len > memorySize)
	{
		return ISL_ERROR;
	}

	return ReadRegisterI2C(static_cast<uint8_t>(addr), data, len);
}

ISL_StatusTypeDef M24C02::Write(uint32_t addr, uint8_t *data, uint16_t len)
{
	if (addr + len > memorySize)
	{
		return ISL_ERROR;
	}

	ISL_StatusTypeDef status = SetRegisterI2C(static_cast<uint8_t>(addr), data, len);
	system::Delay(5);
	return status;
}

ISL_StatusTypeDef M24C02::FullErase()
{
	uint8_t* temp = new uint8_t[16]{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	for (uint8_t i = 0; i < 16; i++)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(Write(i*16, temp, 16));
		system::Delay(5);
	}
	delete[] temp;
	return ISL_OK;
}

}

#endif /* defined(ISL_I2C_ENABLED) and defined(ISL_GPIO_ENABLED) */
