#include <catch2/catch_test_macros.hpp>

#include "Device/I2CDevice.h"
#include "I2CCallHelpers.hpp"
#include "MockI2C.hpp"

using IntroSatLib::test::IsReady;
using IntroSatLib::test::MockI2C;

TEST_CASE("I2CDevice::Init forwards to isReady with device address", "[I2CDevice]")
{
	MockI2C bus;
	bus.expect({
		IsReady(0x3C),
	});
	IntroSatLib::I2CDevice device(bus, 0x3C);

	REQUIRE(device.Init() == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus);
}
