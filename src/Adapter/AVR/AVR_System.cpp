#define INTROSATLIB_INTERNAL
#include "./AVR_System.h"
#ifdef AVR
uint32_t IntroSatLib::system::GetTick() {
	return millis();
}
#endif /* AVR */


