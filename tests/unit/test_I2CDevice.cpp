#include <catch2/catch_test_macros.hpp>

#include "FakeI2C.hpp"
#include "I2CCallHelpers.hpp"
#include "TestableI2CDevice.h"

using IntroSatLib::test::FakeI2C;
using IntroSatLib::test::IsReady;
using IntroSatLib::test::ReadMem;
using IntroSatLib::test::TestableI2CDevice;
using IntroSatLib::test::WriteMem;

TEST_CASE("I2CDevice::Init forwards to isReady with device address", "[I2CDevice]")
{
	FakeI2C bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);
	TestableI2CDevice device(bus, 0x3C);

	REQUIRE(device.Init() == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus, {
		IsReady(0x3C),
	});
}

TEST_CASE("I2CDevice::ReadRegisterI2C forwards to readMem", "[I2CDevice]")
{
	FakeI2C bus;
	bus.enqueueRead({0xAB, 0xCD});
	TestableI2CDevice device(bus, 0x50);

	uint8_t buf[2] = {};
	REQUIRE(device.ReadRegisterI2C(0x10, buf, 2) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE(buf[0] == 0xAB);
	REQUIRE(buf[1] == 0xCD);
	REQUIRE_I2C_CALLS(bus, {
		ReadMem(0x50, 0x10, 2),
	});
}

TEST_CASE("I2CDevice::SetRegisterI2C forwards to writeMem", "[I2CDevice]")
{
	FakeI2C bus;
	TestableI2CDevice device(bus, 0x51);

	uint8_t value = 0x5A;
	REQUIRE(device.SetRegisterI2C(0x22, &value, 1) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus, {
		WriteMem(0x51, 0x22, {0x5A}),
	});
}
