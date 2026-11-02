#define ISL_INTERNAL

#include "Adapter/I2C/I2CAdapter.h"

#ifdef ISL_I2C_ENABLED

#include "MotorFlyWheel.h"

namespace IntroSatLib {

MotorFlyWheel::MotorFlyWheel(interfaces::I2C& bus, uint8_t address): BaseFlyWheel(bus, address)
{
}

MotorFlyWheel::MotorFlyWheel(interfaces::I2C_HANDLE_TYPE& hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
}

MotorFlyWheel::MotorFlyWheel(interfaces::I2C_HANDLE_TYPE* hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
}

MotorFlyWheel::MotorFlyWheel(const MotorFlyWheel &other): BaseFlyWheel(other)
{
}

MotorFlyWheel::MotorFlyWheel(MotorFlyWheel &&other): BaseFlyWheel(other)
{
}

MotorFlyWheel& MotorFlyWheel::operator=(const MotorFlyWheel &other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

MotorFlyWheel& MotorFlyWheel::operator=(MotorFlyWheel &&other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

MotorFlyWheel::~MotorFlyWheel() { }

} /* namespace IntroSatLib */

#endif /* ISL_I2C_ENABLED */