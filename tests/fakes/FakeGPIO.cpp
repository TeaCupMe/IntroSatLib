/**
 * @file FakeGPIO.cpp
 * @brief Recording fake for IntroSatLib::interfaces::GPIO (host unit tests).
 */
#include "Adapter/GPIO.h"

#if defined(ISL_GPIO_ENABLED)

#include "CallLog.h"

using IntroSatLib::test::BusCall;
using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

IntroSatLib::interfaces::GPIO::GPIO(GPIO_HANDLE_TYPE pin) : _pin(pin) {}

uint8_t IntroSatLib::interfaces::GPIO::read() const
{
	CallLog::record(BusCall{BusOp::GPIO_Read, 0, 0, 0, CallLog::gpioLevel(), {}});
	return CallLog::gpioLevel();
}

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{
	const uint8_t level = state ? 1 : 0;
	CallLog::setGpioLevel(level);
	CallLog::record(BusCall{BusOp::GPIO_Write, 0, 0, 0, level, {}});
}

void IntroSatLib::interfaces::GPIO::tone(uint16_t frequency)
{
	CallLog::record(BusCall{BusOp::GPIO_Tone, 0, 0, 0, frequency, {}});
}

void IntroSatLib::interfaces::GPIO::noTone()
{
	CallLog::record(BusCall{BusOp::GPIO_NoTone, 0, 0, 0, 0, {}});
}

bool IntroSatLib::interfaces::GPIO::isValid() const
{
	CallLog::record(BusCall{
		BusOp::GPIO_IsValid,
		0,
		0,
		0,
		CallLog::gpioValid() ? 1u : 0u,
		{},
	});
	return CallLog::gpioValid();
}

#endif /* ISL_GPIO_ENABLED */
