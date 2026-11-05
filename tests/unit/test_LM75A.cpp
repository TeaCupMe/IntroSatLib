#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Device/LM75A/LM75A.h"
#include "I2CCallHelpers.hpp"
#include "MockI2C.hpp"

using IntroSatLib::test::IsReady;
using IntroSatLib::test::MockI2C;
using IntroSatLib::test::ReadMem;

TEST_CASE("LM75A::Init probes device with isReady", "[LM75A]")
{
	MockI2C bus;
	bus.expect({
		IsReady(0x48),
	});
	IntroSatLib::LM75A sensor(bus, 0x48);

	REQUIRE(sensor.Init() == ISL_StatusTypeDef::ISL_OK);
	REQUIRE_I2C_CALLS(bus);
}

TEST_CASE("LM75A::GetRawTemperature reads TEMPERATURE via readMem", "[LM75A]")
{
	MockI2C bus;
	bus.expect({
		ReadMem(0x48, 0x00, 2).respond({0x19, 0x00}),
	});
	IntroSatLib::LM75A sensor(bus, 0x48);

	REQUIRE(sensor.GetRawTemperature() == static_cast<int16_t>(0x1900));
	REQUIRE_I2C_CALLS(bus);
}

TEST_CASE("LM75A::GetTemperature scales raw / 256", "[LM75A]")
{
	MockI2C bus;
	bus.expect({
		ReadMem(0x48, 0x00, 2).respond({0x19, 0x00}),
	});
	IntroSatLib::LM75A sensor(bus);

	REQUIRE(sensor.GetTemperature() == Catch::Approx(25.0f).margin(0.001f));
	REQUIRE_I2C_CALLS(bus);
}

TEST_CASE("LM75A::GetConfig reads CONFIGURATION register", "[LM75A]")
{
	MockI2C bus;
	bus.expect({
		ReadMem(0x49, 0x01, 1).respond({0x42}),
	});
	IntroSatLib::LM75A sensor(bus, 0x49);

	REQUIRE(sensor.GetConfig() == 0x42);
	REQUIRE_I2C_CALLS(bus);
}
