#include <catch2/catch_test_macros.hpp>

#include "CallLog.h"
#include "Device/LSM6DS3/LSM6DS3.h"
#include "FakeI2C.hpp"
#include "I2CCallHelpers.hpp"

using IntroSatLib::test::CallLog;
using IntroSatLib::test::FakeI2C;
using IntroSatLib::test::IsReady;
using IntroSatLib::test::ReadMem;
using IntroSatLib::test::WriteMem;

TEST_CASE("LSM6DS3::SetScaleGyro performs RMW on GYRO_CONFIG", "[LSM6DS3]")
{
	FakeI2C bus;
	bus.enqueueRead({0x00});
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);

	REQUIRE(
		imu.SetScaleGyro(IntroSatLib::LSM6DS3::ScaleGyro::DPS_500)
		== ISL_StatusTypeDef::ISL_OK);

	REQUIRE_I2C_CALLS(bus, {
		ReadMem(0x6B, 0x11, 1),
		WriteMem(0x6B, 0x11, {0x04}),
	});
}

TEST_CASE("LSM6DS3::RawGX reads GYRO_XOUT_H as little-endian int16", "[LSM6DS3]")
{
	FakeI2C bus;
	bus.enqueueRead({0x34, 0x12});
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);

	REQUIRE(imu.RawGX() == static_cast<int16_t>(0x1234));
	REQUIRE_I2C_CALLS(bus, {
		ReadMem(0x6B, 0x22, 2),
	});
}

TEST_CASE("LSM6DS3::InitGyro delays, checks WhoAmI, configures scale/rate", "[LSM6DS3]")
{
	FakeI2C bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);
	bus.enqueueRead({0x69}); // WhoAmI
	bus.enqueueRead({0x00}); // SetScaleGyro
	bus.enqueueRead({0x00}); // SetDataRateGyro

	CallLog::reset();

	IntroSatLib::LSM6DS3 imu(bus, 0x6B);
	REQUIRE(imu.InitGyro() == ISL_StatusTypeDef::ISL_OK);

	REQUIRE_SYSTEM_DELAY(15);
	REQUIRE_I2C_CALLS(bus, {
		IsReady(0x6B),
		ReadMem(0x6B, 0x0F, 1),
		ReadMem(0x6B, 0x11, 1),
		WriteMem(0x6B, 0x11, {0x00}),
		ReadMem(0x6B, 0x11, 1),
		WriteMem(0x6B, 0x11, {0x60}),
		IsReady(0x6B),
	});
}
