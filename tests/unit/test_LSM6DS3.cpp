#include <gtest/gtest.h>

#include "Device/LSM6DS3/LSM6DS3.h"
#include "MockI2C.hpp"

using IntroSatLib::test::MockI2C;
using ::testing::_;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;
using ::testing::StrictMock;

TEST(LSM6DS3, SetsGyroScaleWithReadModifyWrite)
{
	StrictMock<MockI2C> bus;
	InSequence sequence;
	EXPECT_CALL(bus, readMem(0x6B, 0x11, _, 1))
		.WillOnce(DoAll(SetArgPointee<2>(0x00), Return(ISL_OK)));
	EXPECT_CALL(bus, writeMem(0x6B, 0x11, Pointee(0x04), 1)).WillOnce(Return(ISL_OK));
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);
	EXPECT_EQ(imu.SetScaleGyro(IntroSatLib::LSM6DS3::ScaleGyro::DPS_500), ISL_OK);
}

TEST(LSM6DS3, ReadsGyroXAsLittleEndianInt16)
{
	StrictMock<MockI2C> bus;
	const uint8_t response[] = {0x34, 0x12};
	EXPECT_CALL(bus, readMem(0x6B, 0x22, _, 2))
		.WillOnce(DoAll(SetArrayArgument<2>(response, response + 2), Return(ISL_OK)));
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);
	EXPECT_EQ(imu.RawGX(), static_cast<int16_t>(0x1234));
}

TEST(LSM6DS3, InitGyroChecksIdentityAndConfiguresScaleAndRateInOrder)
{
	StrictMock<MockI2C> bus;
	InSequence sequence;
	EXPECT_CALL(bus, isReady(0x6B, 0)).WillOnce(Return(ISL_OK));
	EXPECT_CALL(bus, readMem(0x6B, 0x0F, _, 1))
		.WillOnce(DoAll(SetArgPointee<2>(0x69), Return(ISL_OK)));
	EXPECT_CALL(bus, readMem(0x6B, 0x11, _, 1))
		.WillOnce(DoAll(SetArgPointee<2>(0x00), Return(ISL_OK)));
	EXPECT_CALL(bus, writeMem(0x6B, 0x11, Pointee(0x00), 1)).WillOnce(Return(ISL_OK));
	EXPECT_CALL(bus, readMem(0x6B, 0x11, _, 1))
		.WillOnce(DoAll(SetArgPointee<2>(0x00), Return(ISL_OK)));
	EXPECT_CALL(bus, writeMem(0x6B, 0x11, Pointee(0x60), 1)).WillOnce(Return(ISL_OK));
	EXPECT_CALL(bus, isReady(0x6B, 0)).WillOnce(Return(ISL_OK));
	IntroSatLib::LSM6DS3 imu(bus, 0x6B);
	EXPECT_EQ(imu.InitGyro(), ISL_OK);
}
