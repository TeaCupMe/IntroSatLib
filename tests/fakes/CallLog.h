/**
 * @file CallLog.h
 * @brief Shared recording journal for host GPIO/SPI/UART/System stubs.
 *
 * Driver I2C unit tests use DI MockI2C instead; this log is for Delay and
 * other non-I2C host stubs linked into full-Core host builds.
 */
#ifndef INTROSATLIB_TEST_CALLLOG_H_
#define INTROSATLIB_TEST_CALLLOG_H_

#include "BusCall.h"
#include "IntroSatLib_def.h"

#include <cstdint>
#include <deque>
#include <vector>

namespace IntroSatLib {
namespace test {

/**
 * Thread-hostile global call journal for single-threaded Catch2 runs.
 */
class CallLog {
public:
	static void reset();

	static const std::vector<BusCall>& calls();
	static void record(BusCall call);

	static void setTick(uint32_t tick);
	static uint32_t tick();
	static void advanceTick(uint32_t delta);

	static void setGpioLevel(uint8_t level);
	static uint8_t gpioLevel();
	static void setGpioValid(bool valid);
	static bool gpioValid();

	static void enqueueSpiIn(std::vector<uint8_t> bytes);
	static void fillSpiIn(uint8_t* dest, uint8_t nBytes);

	static void enqueueUartRx(std::vector<uint8_t> bytes);
	static uint8_t uartAvailable();
	static ISL_StatusTypeDef takeUartRx(uint8_t* dest, uint16_t count);

private:
	static std::vector<BusCall>& mutableCalls();
	static std::deque<std::vector<uint8_t>>& spiInQueue();
	static std::deque<uint8_t>& uartRxQueue();
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_CALLLOG_H_ */
