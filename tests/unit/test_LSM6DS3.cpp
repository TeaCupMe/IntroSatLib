#include <catch2/catch_test_macros.hpp>

#include "CallLog.h"
#include "Device/LSM6DS3/LSM6DS3.h"
#include "FakeI2CBus.hpp"

#include <vector>

using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;
using IntroSatLib::test::FakeI2CBus;
using IntroSatLib::test::I2CBusCall;
using IntroSatLib::test::I2CBusOp;

TEST_CASE("LSM6DS3::SetScaleGyro performs RMW on GYRO_CONFIG", "[LSM6DS3]")
{
	FakeI2CBus bus;
	bus.enqueueRead({0x00});
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);

	REQUIRE(
		imu.SetScaleGyro(IntroSatLib::LSM6DS3::ScaleGyro::DPS_500)
		== ISL_StatusTypeDef::ISL_OK);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 2);

	REQUIRE(calls[0].op == I2CBusOp::ReadMem);
	REQUIRE(calls[0].address == 0x6B);
	REQUIRE(calls[0].reg == 0x11);
	REQUIRE(calls[0].nBytes == 1);

	REQUIRE(calls[1].op == I2CBusOp::WriteMem);
	REQUIRE(calls[1].address == 0x6B);
	REQUIRE(calls[1].reg == 0x11);
	REQUIRE(calls[1].nBytes == 1);
	REQUIRE(calls[1].data == std::vector<uint8_t>{0x04});
}

TEST_CASE("LSM6DS3::RawGX reads GYRO_XOUT_H as little-endian int16", "[LSM6DS3]")
{
	FakeI2CBus bus;
	bus.enqueueRead({0x34, 0x12});
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);

	REQUIRE(imu.RawGX() == static_cast<int16_t>(0x1234));

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == I2CBusOp::ReadMem);
	REQUIRE(calls[0].address == 0x6B);
	REQUIRE(calls[0].reg == 0x22);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("LSM6DS3::InitGyro delays, checks WhoAmI, configures scale/rate", "[LSM6DS3]")
{
	FakeI2CBus bus;
	bus.setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);
	bus.enqueueRead({0x69}); // WhoAmI
	bus.enqueueRead({0x00}); // SetScaleGyro
	bus.enqueueRead({0x00}); // SetDataRateGyro

	// system::Delay still uses Phase-1 link FakeSystem / CallLog
	CallLog::reset();

	IntroSatLib::LSM6DS3 imu(bus, 0x6B);
	REQUIRE(imu.InitGyro() == ISL_StatusTypeDef::ISL_OK);

	const auto& sysCalls = CallLog::calls();
	REQUIRE_FALSE(sysCalls.empty());
	REQUIRE(sysCalls[0].op == BusOp::System_Delay);
	REQUIRE(sysCalls[0].value == 15);

	const auto& calls = bus.calls();
	REQUIRE(calls.size() == 7);

	REQUIRE(calls[0].op == I2CBusOp::IsReady);
	REQUIRE(calls[0].address == 0x6B);

	REQUIRE(calls[1].op == I2CBusOp::ReadMem);
	REQUIRE(calls[1].reg == 0x0F);
	REQUIRE(calls[1].nBytes == 1);

	REQUIRE(calls[2].op == I2CBusOp::ReadMem);
	REQUIRE(calls[2].reg == 0x11);
	REQUIRE(calls[3].op == I2CBusOp::WriteMem);
	REQUIRE(calls[3].reg == 0x11);
	REQUIRE(calls[3].data == std::vector<uint8_t>{0x00});

	REQUIRE(calls[4].op == I2CBusOp::ReadMem);
	REQUIRE(calls[4].reg == 0x11);
	REQUIRE(calls[5].op == I2CBusOp::WriteMem);
	REQUIRE(calls[5].reg == 0x11);
	REQUIRE(calls[5].data == std::vector<uint8_t>{0x60});

	REQUIRE(calls[6].op == I2CBusOp::IsReady);
	REQUIRE(calls[6].address == 0x6B);
}
