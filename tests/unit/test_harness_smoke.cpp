#include <catch2/catch_test_macros.hpp>

#include "Adapter/I2C.h"
#include "CallLog.h"

#ifndef ISL_I2C_ENABLED
#error "ISL_I2C_ENABLED must be defined for host unit tests (HAL I2C stubs missing)"
#endif

TEST_CASE("host harness enables I2C and records isReady", "[harness]")
{
	IntroSatLib::test::CallLog::reset();
	IntroSatLib::test::CallLog::setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	IntroSatLib::interfaces::I2C bus(&handle);

	REQUIRE(bus.isReady(0x48) == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = IntroSatLib::test::CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == IntroSatLib::test::BusOp::I2C_IsReady);
	REQUIRE(calls[0].address == 0x48);
}
