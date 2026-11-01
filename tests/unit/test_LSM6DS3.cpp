#include <catch2/catch_test_macros.hpp>

#include "CallLog.h"
#include "Device/LSM6DS3/LSM6DS3.h"

#include <vector>

using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

namespace {

IntroSatLib::LSM6DS3 makeImu(IntroSatLib::interfaces::I2C_HANDLE_TYPE& handle)
{
	return IntroSatLib::LSM6DS3(IntroSatLib::interfaces::I2C(&handle), 0x6B);
}

bool isI2cOp(IntroSatLib::test::BusOp op)
{
	return op == BusOp::I2C_IsReady || op == BusOp::I2C_ReadMem
		|| op == BusOp::I2C_WriteMem || op == BusOp::I2C_Read
		|| op == BusOp::I2C_Write;
}

} /* namespace */

TEST_CASE("LSM6DS3::SetScaleGyro performs RMW on GYRO_CONFIG", "[LSM6DS3]")
{
	CallLog::reset();
	CallLog::enqueueRead({0x00});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto imu = makeImu(handle);

	REQUIRE(
		imu.SetScaleGyro(IntroSatLib::LSM6DS3::ScaleGyro::DPS_500)
		== ISL_StatusTypeDef::ISL_OK);

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 2);

	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].address == 0x6B);
	REQUIRE(calls[0].reg == 0x11);
	REQUIRE(calls[0].nBytes == 1);

	REQUIRE(calls[1].op == BusOp::I2C_WriteMem);
	REQUIRE(calls[1].address == 0x6B);
	REQUIRE(calls[1].reg == 0x11);
	REQUIRE(calls[1].nBytes == 1);
	REQUIRE(calls[1].data == std::vector<uint8_t>{0x04});
}

TEST_CASE("LSM6DS3::RawGX reads GYRO_XOUT_H as little-endian int16", "[LSM6DS3]")
{
	CallLog::reset();
	// RegisterMap starts at GYRO_XOUT_H; driver stores buf[1]<<8 | buf[0]
	CallLog::enqueueRead({0x34, 0x12});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto imu = makeImu(handle);

	REQUIRE(imu.RawGX() == static_cast<int16_t>(0x1234));

	const auto& calls = CallLog::calls();
	REQUIRE(calls.size() == 1);
	REQUIRE(calls[0].op == BusOp::I2C_ReadMem);
	REQUIRE(calls[0].address == 0x6B);
	REQUIRE(calls[0].reg == 0x22);
	REQUIRE(calls[0].nBytes == 2);
}

TEST_CASE("LSM6DS3::InitGyro delays, checks WhoAmI, configures scale/rate", "[LSM6DS3]")
{
	CallLog::reset();
	CallLog::setIsReadyStatus(ISL_StatusTypeDef::ISL_OK);
	// WhoAmI
	CallLog::enqueueRead({0x69});
	// SetScaleGyro (default DPS_245): read then write
	CallLog::enqueueRead({0x00});
	// SetDataRateGyro (DR_G_416_Hz = 6): read then write
	CallLog::enqueueRead({0x00});

	IntroSatLib::interfaces::I2C_HANDLE_TYPE handle{};
	auto imu = makeImu(handle);

	REQUIRE(imu.InitGyro() == ISL_StatusTypeDef::ISL_OK);

	const auto& calls = CallLog::calls();
	REQUIRE_FALSE(calls.empty());
	REQUIRE(calls[0].op == BusOp::System_Delay);
	REQUIRE(calls[0].value == 15);

	std::vector<IntroSatLib::test::BusCall> i2cCalls;
	for (const auto& call : calls) {
		if (isI2cOp(call.op)) {
			i2cCalls.push_back(call);
		}
	}

	// isReady, WhoAmI readMem, scale RMW, rate RMW, final isReady
	REQUIRE(i2cCalls.size() == 7);

	REQUIRE(i2cCalls[0].op == BusOp::I2C_IsReady);
	REQUIRE(i2cCalls[0].address == 0x6B);

	REQUIRE(i2cCalls[1].op == BusOp::I2C_ReadMem);
	REQUIRE(i2cCalls[1].reg == 0x0F);
	REQUIRE(i2cCalls[1].nBytes == 1);

	REQUIRE(i2cCalls[2].op == BusOp::I2C_ReadMem);
	REQUIRE(i2cCalls[2].reg == 0x11);
	REQUIRE(i2cCalls[3].op == BusOp::I2C_WriteMem);
	REQUIRE(i2cCalls[3].reg == 0x11);
	REQUIRE(i2cCalls[3].data == std::vector<uint8_t>{0x00}); // DPS_245 << 2

	REQUIRE(i2cCalls[4].op == BusOp::I2C_ReadMem);
	REQUIRE(i2cCalls[4].reg == 0x11);
	REQUIRE(i2cCalls[5].op == BusOp::I2C_WriteMem);
	REQUIRE(i2cCalls[5].reg == 0x11);
	// DR_G_416_Hz = 6 -> bits[7:4] = 0x60
	REQUIRE(i2cCalls[5].data == std::vector<uint8_t>{0x60});

	REQUIRE(i2cCalls[6].op == BusOp::I2C_IsReady);
	REQUIRE(i2cCalls[6].address == 0x6B);
}
