#define INTROSATLIB_INTERNAL
#include "Adapter/System.h"

uint32_t IntroSatLib::system::GetTick() {
	return HAL_GetTick();
}
