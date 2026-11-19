#ifndef DEVICE_M24C02_H_
#define DEVICE_M24C02_H_

#include "Adapter/I2C/I2CAdapter.h"
#include "Adapter/GPIO.h"
#include "Device/I2CDevice.h"
#include "Device/Base/BaseMemoryDriver.h"
#include "IntroSatLib_def.h"

#include <cstddef>

namespace IntroSatLib
{

class M24C02 : I2CDevice, public BaseMemoryDriver
{
private:
	static constexpr uint8_t BASE_ADDRESS = 0x50;
	static constexpr size_t MEMORY_SIZE = 256;
	interfaces::GPIO wcPin;
	bool readOnly{false};

public:
	M24C02(interfaces::I2C& bus, interfaces::GPIO_HANDLE_TYPE wc, uint8_t address = BASE_ADDRESS):
		I2CDevice(bus, address),
		wcPin(wc)
	{
		memorySize = MEMORY_SIZE;
	}

	M24C02(interfaces::I2C_HANDLE_TYPE& hi2c, interfaces::GPIO_HANDLE_TYPE wc, uint8_t address = BASE_ADDRESS):
		I2CDevice(hi2c, address),
		wcPin(wc)
	{
		memorySize = MEMORY_SIZE;
	}

	M24C02(interfaces::I2C_HANDLE_TYPE* hi2c, interfaces::GPIO_HANDLE_TYPE wc, uint8_t address = BASE_ADDRESS):
		I2CDevice(hi2c, address),
		wcPin(wc)
	{
		memorySize = MEMORY_SIZE;
	}

	ISL_StatusTypeDef Init() override
	{
		return Init(false);
	}

	ISL_StatusTypeDef Init(bool readOnly_);

	ISL_StatusTypeDef Read(uint32_t addr, uint8_t* data, uint16_t len) override;
	ISL_StatusTypeDef Write(uint32_t addr, uint8_t* data, uint16_t len) override;
	ISL_StatusTypeDef FullErase() override;

	void enableWrite() { if (!readOnly) wcPin.reset(); }
	void disableWrite() { wcPin.set(); }
};

}

#endif /* DEVICE_M24C02_H_ */
