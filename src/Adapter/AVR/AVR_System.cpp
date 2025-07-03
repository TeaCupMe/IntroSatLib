#define INTROSATLIB_INTERNAL
#include "Adapter/System.h"
#ifdef AVR
uint32_t IntroSatLib::system::GetTick() {
	return millis();
}
#endif /* AVR */


