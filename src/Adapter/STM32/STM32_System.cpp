#define INTROSATLIB_INTERNAL
#include "Adapter/System.h"
#include "STM32_System.h"

uint32_t IntroSatLib::system::GetTick() {
	return HAL_GetTick();
}
