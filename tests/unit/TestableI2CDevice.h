#ifndef INTROSATLIB_TEST_TESTABLE_I2CDEVICE_H_
#define INTROSATLIB_TEST_TESTABLE_I2CDEVICE_H_

#include "Device/I2CDevice.h"

namespace IntroSatLib {
namespace test {

/** Exposes protected I2CDevice helpers for forwarding assertions. */
class TestableI2CDevice : public I2CDevice {
public:
	using I2CDevice::I2CDevice;
	using I2CDevice::Init;
	using I2CDevice::IsReady;
	using I2CDevice::ReadI2C;
	using I2CDevice::ReadRegisterI2C;
	using I2CDevice::SetRegisterI2C;
	using I2CDevice::WriteI2C;
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_TESTABLE_I2CDEVICE_H_ */
