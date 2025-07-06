#define INTROSATLIB_INTERNAL
#include "./AVR_System.h"

uint32_t IntroSatLib::system::GetTick() {
	return millis();
}


