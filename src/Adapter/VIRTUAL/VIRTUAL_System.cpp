#define ISL_INTERNAL
#include "Adapter/System.h"

#if defined(ISL_VIRTUAL)

#include <thread>
#include <chrono>

std::chrono::time_point<std::chrono::steady_clock> __execution_start_time__;


uint32_t IntroSatLib::system::GetTick() {
	return (uint32_t)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - __execution_start_time__).count());
}

void IntroSatLib::system::Delay(uint32_t milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

#endif /* ARDUINO */


