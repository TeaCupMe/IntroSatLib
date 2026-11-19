#include <gtest/gtest.h>

#include "Accelerometer.h"
#include "AK8963.h"
#include "Gyroscope.h"
#include "LightSensor.h"
#include "CoilFlyWheel.h"
#include "MotorFlyWheel.h"
#include "Device/ADS7830/ADS7830.h"
#include "Device/LIS2MDL/LIS2MDL.h"
#include "Device/LIS3MDL/LIS3MDL.h"
#include "Device/MS5611/MS5611.h"
#include "MockI2C.hpp"

TEST(I2CDrivers, LinkWithInjectedBusWithoutPlatformAdapters)
{
	// Any bus call during construction fails through StrictMock.
	::testing::StrictMock<IntroSatLib::test::MockI2C> bus;
	IntroSatLib::Accelerometer accelerometer(bus);
	IntroSatLib::AK8963 magnetometer(bus);
	IntroSatLib::Gyroscope gyroscope(bus);
	IntroSatLib::LightSensor light(bus);
	IntroSatLib::CoilFlyWheel coil(bus);
	IntroSatLib::MotorFlyWheel motor(bus);
	IntroSatLib::ADS7830 adc(bus);
	IntroSatLib::LIS2MDL lis2(bus);
	IntroSatLib::LIS3MDL lis3(bus);
	IntroSatLib::MS5611 barometer(bus);
}
