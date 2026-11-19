#include <gtest/gtest.h>

#include "Device/I2CDevice.h"
#include "MockI2C.hpp"

using IntroSatLib::test::MockI2C;
using ::testing::Return;
using ::testing::StrictMock;

TEST(I2CDevice, InitProbesDeviceAddress)
{
	StrictMock<MockI2C> bus;
	EXPECT_CALL(bus, isReady(0x3C, 0)).WillOnce(Return(ISL_OK));
	IntroSatLib::I2CDevice device(bus, 0x3C);
	EXPECT_EQ(device.Init(), ISL_OK);
}

class I2CDeviceFailure : public ::testing::TestWithParam<ISL_StatusTypeDef> {};

TEST_P(I2CDeviceFailure, InitPreservesBusStatus)
{
	StrictMock<MockI2C> bus;
	EXPECT_CALL(bus, isReady(0x3C, 0)).WillOnce(Return(GetParam()));
	IntroSatLib::I2CDevice device(bus, 0x3C);
	EXPECT_EQ(device.Init(), GetParam());
}

INSTANTIATE_TEST_SUITE_P(BusErrors, I2CDeviceFailure,
	::testing::Values(ISL_ERROR, ISL_BUSY, ISL_TIMEOUT),
	[](const ::testing::TestParamInfo<ISL_StatusTypeDef>& info) {
		switch (info.param) {
		case ISL_ERROR: return "Error";
		case ISL_BUSY: return "Busy";
		default: return "Timeout";
		}
	});
