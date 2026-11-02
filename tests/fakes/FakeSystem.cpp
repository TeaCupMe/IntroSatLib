/**
 * @file FakeSystem.cpp
 * @brief Recording fake for IntroSatLib::system::* (host unit tests).
 */
#define ISL_INTERNAL
#include "Adapter/System.h"

#include "CallLog.h"

using IntroSatLib::test::BusCall;
using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

uint32_t IntroSatLib::system::GetTick()
{
	CallLog::record(BusCall{BusOp::System_GetTick, 0, 0, 0, CallLog::tick(), {}});
	return CallLog::tick();
}

uint32_t IntroSatLib::system::GetMicros()
{
	const uint32_t micros = CallLog::tick() * 1000u;
	CallLog::record(BusCall{BusOp::System_GetMicros, 0, 0, 0, micros, {}});
	return micros;
}

void IntroSatLib::system::Delay(uint32_t milliseconds)
{
	CallLog::advanceTick(milliseconds);
	CallLog::record(BusCall{BusOp::System_Delay, 0, 0, 0, milliseconds, {}});
}

void IntroSatLib::system::DelayMicroseconds(uint32_t microseconds)
{
	CallLog::record(BusCall{BusOp::System_DelayMicroseconds, 0, 0, 0, microseconds, {}});
}
