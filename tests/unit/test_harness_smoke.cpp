#include <gtest/gtest.h>

#include "Adapter/I2C/I2CAdapter.h"
#include "MockI2C.hpp"

#ifndef ISL_I2C_ENABLED
#error "ISL_I2C_ENABLED must be defined for host unit tests"
#endif

TEST(HostHarness, InjectsMockThroughPublicI2CInterface)
{
	::testing::StrictMock<IntroSatLib::test::MockI2C> bus;
	EXPECT_CALL(bus, isReady(0x48, 0)).WillOnce(::testing::Return(ISL_OK));
	IntroSatLib::interfaces::I2C& ref = bus;
	EXPECT_EQ(ref.isReady(0x48), ISL_OK);
}
