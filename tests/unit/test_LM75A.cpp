#include <gtest/gtest.h>

#include "Device/LM75A/LM75A.h"
#include "MockI2C.hpp"

using IntroSatLib::test::MockI2C;
using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;
using ::testing::StrictMock;

TEST(LM75A, InitProbesDevice)
{
	StrictMock<MockI2C> bus;
	EXPECT_CALL(bus, isReady(0x48, 0)).WillOnce(Return(ISL_OK));
	IntroSatLib::LM75A sensor(bus, 0x48);
	EXPECT_EQ(sensor.Init(), ISL_OK);
}

TEST(LM75A, ReadsRawTemperatureRegister)
{
	StrictMock<MockI2C> bus;
	const uint8_t response[] = {0x19, 0x00};
	EXPECT_CALL(bus, readMem(0x48, 0x00, _, 2))
		.WillOnce(DoAll(SetArrayArgument<2>(response, response + 2), Return(ISL_OK)));
	IntroSatLib::LM75A sensor(bus, 0x48);
	EXPECT_EQ(sensor.GetRawTemperature(), static_cast<int16_t>(0x1900));
}

TEST(LM75A, ScalesRawTemperatureBy256)
{
	StrictMock<MockI2C> bus;
	const uint8_t response[] = {0x19, 0x00};
	EXPECT_CALL(bus, readMem(0x48, 0x00, _, 2))
		.WillOnce(DoAll(SetArrayArgument<2>(response, response + 2), Return(ISL_OK)));
	IntroSatLib::LM75A sensor(bus);
	EXPECT_NEAR(sensor.GetTemperature(), 25.0f, 0.001f);
}

TEST(LM75A, ReadsConfigurationAtCustomAddress)
{
	StrictMock<MockI2C> bus;
	EXPECT_CALL(bus, readMem(0x49, 0x01, _, 1))
		.WillOnce(DoAll(SetArgPointee<2>(0x42), Return(ISL_OK)));
	IntroSatLib::LM75A sensor(bus, 0x49);
	EXPECT_EQ(sensor.GetConfig(), 0x42);
}
