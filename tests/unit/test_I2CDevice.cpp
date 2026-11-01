#include <catch2/catch_test_macros.hpp>

#include "CallLog.h"
#include "TestableI2CDevice.h"

#include <vector>

using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;
using IntroSatLib::test::TestableI2CDevice;

namespace {

IntroSatLib::interfaces::I2C makeBus(IntroSatLib::interfaces::I2C_HANDLE_TYPE& handle)
{
	return IntroSatLib::interfaces::I2C(&handle);
}

} /* namespace */

TEST_CASE("I2CDevice::Init forwards to isReady with device address", "[I2CDevice]")
{
	CallLog::reset();
	CallLog::setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	TestableI2CDevice device(makeBus(handle), 0x3C);

	REQUIRE(device.Init() == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_IsReady);
	REQUIRE(calls[0].address == 0x3C);
}

TEST_CASE("I2CDevice::ReadRegisterI2C forwards to readMem", "[I2CDevice]")
{
	CallLog::reset();
	CallLog::enqueueRead({0xAB, 0xCD});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	TestableI2CDevice device(makeBus(handle), 0x50);

	uint8_t buf[2] = {};
	REQUIRE(device.ReadRegisterI2C(0x10, buf, 2) == ISL_StatusTypeDef::ISL_OK);
	REQUIRE(buf[0] == 0xAB);
	REQUIRE(buf[1] == 0xCD);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].address == 0x50);
	REQUIRE(calls[0].reg == 0x10);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("I2CDevice::SetRegisterI2C forwards to writeMem", "[I2CDevice]")
{
	CallLog::reset();

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	TestableI2CDevice device(makeBus(handle), 0x51);

	uint8_t value = 0x5A;
	REQUIRE(device.SetRegisterI2C(0x22, &value, 1) == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_WriteMem);
	REQUIRE(calls[0].address == 0x51);
	REQUIRE(calls[0].reg == 0x22);
	REQUIRE(calls[0].nBytes == 1);
	REQUIRE(calls[0].data == std::vector<uint8_t>{0x5A});
}
