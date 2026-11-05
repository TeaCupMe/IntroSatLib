#include <catch2/catch_test_macros.hpp>

#include "Adapter/I2C/I2CAdapter.h"
#include "I2CCallHelpers.hpp"
#include "MockI2C.hpp"

#ifndef ISL_I2C_ENABLED
#error "ISL_I2C_ENABLED must be defined for host unit tests (HAL I2C stubs missing)"
#endif

using IntroSatLib::test::IsReady;
using IntroSatLib::test::MockI2C;

TEST_CASE("host harness exposes I2C and MockI2C records isReady", "[harness]")
{
	MockI2C bus;
	bus.expect({
		IsReady(0x48),
	});

	IntroSatLib::interfaces::I2C& ref = bus;
	REQUIRE(ref.isReady(0x48) == ISL_StatusTypeDef::ISL_OK);

	REQUIRE_I2C_CALLS(bus);
}
