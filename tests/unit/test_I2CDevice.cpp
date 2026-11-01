#include <catch2/catch_test_macros.hpp>

#include "FakeI2CBus.hpp"
#include "TestableI2CDevice.h"

#include <vector>

using IntroSatLib::test::FakeI2CBus;
using IntroSatLib::test::I2CBusOp;
using IntroSatLib::test::TestableI2CDevice;

TEST_CASE("I2CDevice::Init forwards to isReady with device address", "[I2CDevice]")
{
	FakeI2CBus bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);
	TestableI2CDevice device(bus, 0x3C);

	REQUIRE(device.Init() == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == I2CBusOp::IsReady);
	REQUIRE(calls[0].address == 0x3C);
}

TEST_CASE("I2CDevice::ReadRegisterI2C forwards to readMem", "[I2CDevice]")
{
	FakeI2CBus bus;
	bus.enqueueRead({0xAB, 0xCD});
	TestableI2CDevice device(bus, 0x50);

	uint8_t buf[2] = {};
	REQUIRE(device.ReadRegisterI2C(0x10, buf, 2) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE(buf[0] == 0xAB);
	REQUIRE(buf[1] == 0xCD);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == I2CBusOp::ReadMem);
	REQUIRE(calls[0].address == 0x50);
	REQUIRE(calls[0].reg == 0x10);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("I2CDevice::SetRegisterI2C forwards to writeMem", "[I2CDevice]")
{
	FakeI2CBus bus;
	TestableI2CDevice device(bus, 0x51);

	uint8_t value = 0x5A;
	REQUIRE(device.SetRegisterI2C(0x22, &value, 1) == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == I2CBusOp::WriteMem);
	REQUIRE(calls[0].address == 0x51);
	REQUIRE(calls[0].reg == 0x22);
	REQUIRE(calls[0].nBytes == 1);
	REQUIRE(calls[0].data == std::vector<uint8_t>{0x5A});
}
