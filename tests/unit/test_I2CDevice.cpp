#include <catch2/catch_test_macros.hpp>

#include "I2CCallHelpers.hpp"
#include "MockI2C.hpp"
#include "TestableI2CDevice.h"

using IntroSatLib::test::IsReady;
using IntroSatLib::test::MockI2C;
using IntroSatLib::test::ReadMem;
using IntroSatLib::test::TestableI2CDevice;
using IntroSatLib::test::WriteMem;

TEST_CASE("I2CDevice::Init forwards to isReady with device address", "[I2CDevice]")
{
	MockI2C bus;
	bus.expect({
		IsReady(0x3C),
	});
	TestableI2CDevice device(bus, 0x3C);

	REQUIRE(device.Init() == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus);
}

TEST_CASE("I2CDevice::ReadRegisterI2C forwards to readMem", "[I2CDevice]")
{
	MockI2C bus;
	bus.expect({
		ReadMem(0x50, 0x10, 2).respond({0xAB, 0xCD}),
	});
	TestableI2CDevice device(bus, 0x50);

	uint8_t buf[2] = {};
	REQUIRE(device.ReadRegisterI2C(0x10, buf, 2) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE(buf[0] == 0xAB);
	REQUIRE(buf[1] == 0xCD);
	REQUIRE_I2C_CALLS(bus);
}

TEST_CASE("I2CDevice::SetRegisterI2C forwards to writeMem", "[I2CDevice]")
{
	MockI2C bus;
	bus.expect({
		WriteMem(0x51, 0x22, {0x5A}),
	});
	TestableI2CDevice device(bus, 0x51);

	uint8_t value = 0x5A;
	REQUIRE(device.SetRegisterI2C(0x22, &value, 1) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus);
}
