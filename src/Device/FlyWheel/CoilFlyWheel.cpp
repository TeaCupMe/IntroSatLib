#define ISL_INTERNAL

#include "Adapter/I2C/I2CAdapter.h"

#ifdef ISL_I2C_ENABLED

#include "CoilFlyWheel.h"

namespace IntroSatLib {

CoilFlyWheel::CoilFlyWheel(interfaces::I2C& bus, uint8_t address): BaseFlyWheel(bus, address)
{
}

CoilFlyWheel::CoilFlyWheel(interfaces::I2C_HANDLE_TYPE& hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
}

CoilFlyWheel::CoilFlyWheel(interfaces::I2C_HANDLE_TYPE* hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
}

CoilFlyWheel::CoilFlyWheel(const CoilFlyWheel &other): BaseFlyWheel(other)
{
}

CoilFlyWheel::CoilFlyWheel(CoilFlyWheel &&other): BaseFlyWheel(other)
{
}

CoilFlyWheel& CoilFlyWheel::operator=(const CoilFlyWheel &other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

CoilFlyWheel& CoilFlyWheel::operator=(CoilFlyWheel &&other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

CoilFlyWheel::~CoilFlyWheel() { }

} /* namespace IntroSatLib */

#endif /* ISL_I2C_ENABLED */