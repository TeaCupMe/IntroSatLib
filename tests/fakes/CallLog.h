/**
 * @file CallLog.h
 * @brief Shared recording journal for fake Adapter implementations.
 */
#ifndef INTROSATLIB_TEST_CALLLOG_H_
#define INTROSATLIB_TEST_CALLLOG_H_

#include "IntroSatLib_def.h"

#include <cstdint>
#include <deque>
#include <vector>

namespace IntroSatLib {
namespace test {

enum class BusOp : uint8_t {
	I2C_IsReady,
	I2C_Read,
	I2C_Write,
	I2C_ReadMem,
	I2C_WriteMem,
	SPI_Transfer,
	GPIO_Read,
	GPIO_Write,
	GPIO_IsValid,
	GPIO_Tone,
	GPIO_NoTone,
	System_GetTick,
	System_GetMicros,
	System_Delay,
	System_DelayMicroseconds,
	UART_Available,
	UART_Receive,
	UART_Transmit,
	UART_TransmitAsync,
};

struct BusCall {
	BusOp op{};
	uint8_t address = 0;
	uint8_t reg = 0;
	uint8_t nBytes = 0;
	uint32_t value = 0;
	std::vector<uint8_t> data;
};

/**
 * Thread-hostile global call journal for single-threaded Catch2 runs.
 * Fake adapters record here; tests reset/program responses before acting.
 */
class CallLog {
public:
	static void reset();

	static const std::vector<BusCall>& calls();
	static void record(BusCall call);

	static void setIsReadyStatus(ISL_StatusTypeDef status);
	static ISL_StatusTypeDef takeIsReadyStatus();

	static void enqueueRead(std::vector<uint8_t> bytes);
	static void fillRead(uint8_t* dest, uint8_t nBytes);

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
	static std::deque<std::vector<uint8_t>>& readQueue();
	static std::deque<std::vector<uint8_t>>& spiInQueue();
	static std::deque<uint8_t>& uartRxQueue();
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_CALLLOG_H_ */
