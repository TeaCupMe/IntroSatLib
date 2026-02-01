#define ISL_INTERNAL
#include "Adapter/System.h"
#if defined(ISL_VIRTUAL)
#include <thread>
#include <chrono>

// uint32_t IntroSatLib::system::GetTick() {
// 	return millis();
// }

void IntroSatLib::system::Delay(uint32_t milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

#endif /* ARDUINO */


