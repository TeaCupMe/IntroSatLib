#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "CallLog.h"
#include "Device/LM75A/LM75A.h"

using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

namespace {

IntroSatLib::LM75A makeSensor(
	IntroSatLib::interfaces::I2C_HANDLE_TYPE& handle,
	uint8_t address = 0x48)
{
	return IntroSatLib::LM75A(IntroSatLib::interfaces::I2C(&handle), address);
}

} /* namespace */

TEST_CASE("LM75A::Init probes device with isReady", "[LM75A]")
{
	CallLog::reset();
	CallLog::setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto sensor = makeSensor(handle);

	REQUIRE(sensor.Init() == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_IsReady);
	REQUIRE(calls[0].address == 0x48);
}

TEST_CASE("LM75A::GetRawTemperature reads TEMPERATURE via readMem", "[LM75A]")
{
	CallLog::reset();
	CallLog::enqueueRead({0x19, 0x00});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto sensor = makeSensor(handle);

	REQUIRE(sensor.GetRawTemperature() == static_cast<int16_t>(0x1900));

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].address == 0x48);
	REQUIRE(calls[0].reg == 0x00);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("LM75A::GetTemperature scales raw / 256", "[LM75A]")
{
	CallLog::reset();
	CallLog::enqueueRead({0x19, 0x00});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto sensor = makeSensor(handle);

	REQUIRE(sensor.GetTemperature() == Catch::Approx(25.0f).margin(0.001f));

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].reg == 0x00);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("LM75A::GetConfig reads CONFIGURATION register", "[LM75A]")
{
	CallLog::reset();
	CallLog::enqueueRead({0x42});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto sensor = makeSensor(handle, 0x49);

	REQUIRE(sensor.GetConfig() == 0x42);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].address == 0x49);
	REQUIRE(calls[0].reg == 0x01);
	REQUIRE(calls[0].nBytes == 1);
}
