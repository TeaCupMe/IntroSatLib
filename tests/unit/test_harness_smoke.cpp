#include <catch2/catch_test_macros.hpp>

#include "Adapter/I2C.h"
#include "Adapter/I2CBus.h"
#include "FakeI2CBus.hpp"

#ifndef ISL_I2C_ENABLED
#error "ISL_I2C_ENABLED must be defined for host unit tests (HAL I2C stubs missing)"
#endif

TEST_CASE("host harness exposes I2CBus and FakeI2CBus records isReady", "[harness]")
{
	IntroSatLib::test::FakeI2CBus bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);

	IntroSatLib::interfaces::I2CBus& ref = bus;
	REQUIRE(ref.isReady(0x48) == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == IntroSatLib::test::I2CBusOp::IsReady);
	REQUIRE(calls[0].address == 0x48);
}
