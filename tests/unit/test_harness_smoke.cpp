#include <catch2/catch_test_macros.hpp>

#include "Adapter/I2C/I2CAdapter.h"
#include "FakeI2C.hpp"
#include "I2CCallHelpers.hpp"

#ifndef ISL_I2C_ENABLED
#error "ISL_I2C_ENABLED must be defined for host unit tests (HAL I2C stubs missing)"
#endif

using IntroSatLib::test::FakeI2C;
using IntroSatLib::test::IsReady;

TEST_CASE("host harness exposes I2C and FakeI2C records isReady", "[harness]")
{
	FakeI2C bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);

	IntroSatLib::interfaces::I2C& ref = bus;
	REQUIRE(ref.isReady(0x48) == ISL_StatusTypeDef::ISL_OK);

	REQUIRE_I2C_CALLS(bus, {
		IsReady(0x48),
	});
}
