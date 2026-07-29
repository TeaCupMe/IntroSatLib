#define ISL_INTERNAL
#include "Adapter/System.h"

#if defined(HAL_MODULE_ENABLED) and !defined(ARDUINO)
#include "stm32xxyy_hal.h"

uint32_t IntroSatLib::system::GetTick() {
	return HAL_GetTick();
}

void IntroSatLib::system::Delay(uint32_t milliseconds) {
	HAL_Delay(milliseconds);
}

uint32_t IntroSatLib::system::GetMicros() {
	uint32_t ms;
    uint32_t st;

    do
    {
        ms = HAL_GetTick();
        st = SysTick->VAL;
        asm volatile("nop");
        asm volatile("nop");
    } while (ms != HAL_GetTick());

    return (ms + 1) * 1000 - st / ((SysTick->LOAD + 1) / 1000);
}

void IntroSatLib::system::DelayMicroseconds(uint32_t microseconds) {
	if (microseconds == 0) {
		return;
	}
	
#ifdef defined(DWT_BASE) && !defined(DWT_DELAY_DISABLED)
	uint32_t start = DWT->CYCCNT;
	// Number of cycles to wait for
	uint32_t cycles = microseconds * (HAL_RCC_GetHCLKFreq() / 1000000);
	while ((DWT->CYCCNT - start) < cycles);
#else
	__IO uint32_t currentTicks = SysTick->VAL;
	static const uint32_t ticksPerMs = SysTick->LOAD + 1;
	// const uint32_t ticks = ((microseconds - ((microseconds > 0) ? 1 : 0)) * ticksPerMs) / 1000;
	const uint32_t ticks = ((microseconds - 1) * ticksPerMs) / 1000;

	uint32_t elapsedTicks = 0;
	__IO uint32_t oldTicks = currentTicks;

	do {
		currentTicks = SysTick->VAL;
		elapsedTicks += (oldTicks < currentTicks) ? (ticksPerMs + oldTicks - currentTicks) : (oldTicks - currentTicks);
		oldTicks = currentTicks;
	} while (ticks > elapsedTicks);

#endif
}
#endif /* defined(HAL_MODULE_ENABLED) && !defined(ARDUINO) */
