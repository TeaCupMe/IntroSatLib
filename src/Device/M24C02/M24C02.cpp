
#include "IntroSatLib_def.h"
#define ISL_INTERNAL

#include "Adapter/I2C.h"
#include "Adapter/GPIO.h"
#include "Adapter/System.h"

#if defined(ISL_I2C_ENABLED) and defined(ISL_GPIO_ENABLED)

#include <Device/M24C02/M24C02.h>

namespace IntroSatLib {

ISL_StatusTypeDef M24C02::Init(bool readOnly_)
{
	readOnly = readOnly_;
	
	// If not read-only, check that WC pin is walid and assert it.
	if (!readOnly)
	{
		if (!wcPin.isValid()) return ISL_StatusTypeDef::ISL_ERROR;
		wcPin.set();
	}

	return I2CDevice::Init();
}

ISL_StatusTypeDef M24C02::Read(uint32_t addr, uint8_t *data, uint16_t len)
{
	if (addr + len > memorySize)
	{
		return ISL_ERROR;
	}

	// Split into 16-byte frames, since STM32 I2C buffer has only 16 bytes
	size_t fullFrames = len / 16;

	for (size_t i{0}; i < fullFrames; ++i)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(static_cast<uint8_t>(addr) + i*16, &data[i*16], 16));
	}

	return ReadRegisterI2C(static_cast<uint8_t>(addr) + fullFrames * 16, &data[fullFrames * 16], len - 16*fullFrames);
}

ISL_StatusTypeDef M24C02::Write(uint32_t addr, uint8_t *data, uint16_t len)
{
	if (readOnly) return ISL_StatusTypeDef::ISL_ERROR;

	if (addr + len > memorySize)
	{
		return ISL_ERROR;
	}

	// Split into 16-byte frames, since STM32 I2C buffer has only 16 bytes
	size_t fullFrames = len / 16;
	ISL_StatusTypeDef status = ISL_OK;

	for (size_t i{0}; i < fullFrames; ++i)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(static_cast<uint8_t>(addr) + i*16, &data[i*16], 16));
		system::Delay(5);
	}

	const uint16_t remainder = static_cast<uint16_t>(len - 16 * fullFrames);
	if ((remainder > 0) && status == ISL_StatusTypeDef::ISL_OK)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(static_cast<uint8_t>(addr) + fullFrames * 16, &data[fullFrames * 16], remainder));
		system::Delay(5);
	}

	return status;
}

ISL_StatusTypeDef M24C02::FullErase()
{
	if (readOnly) return ISL_StatusTypeDef::ISL_ERROR;

	uint8_t temp[16]{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
	for (uint8_t i = 0; i < 16; i++)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(Write(i*16, temp, 16));
	}
	return ISL_OK;
}

}

#endif /* defined(ISL_I2C_ENABLED) and defined(ISL_GPIO_ENABLED) */
