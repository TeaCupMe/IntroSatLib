/**
 * @file HostSystem.cpp
 * @brief No-op system::* for host MockI2C unit tests (Delay used by some drivers).
 */
#define ISL_INTERNAL
#include "Adapter/System.h"

uint32_t IntroSatLib::system::GetTick()
{
	return 0;
}

uint32_t IntroSatLib::system::GetMicros()
{
	return 0;
}

void IntroSatLib::system::Delay(uint32_t /*milliseconds*/)
{
}

void IntroSatLib::system::DelayMicroseconds(uint32_t /*microseconds*/)
{
}
