#include "CallLog.h"

#include <algorithm>
#include <cstring>

namespace IntroSatLib {
namespace test {
namespace {

ISL_StatusTypeDef g_isReadyStatus = ISL_StatusTypeDef::ISL_OK;
uint32_t g_tick = 0;
uint8_t g_gpioLevel = 0;
bool g_gpioValid = true;

} /* namespace */

std::vector<BusCall>& CallLog::mutableCalls()
{
	static std::vector<BusCall> calls;
	return calls;
}

std::deque<std::vector<uint8_t>>& CallLog::readQueue()
{
	static std::deque<std::vector<uint8_t>> queue;
	return queue;
}

std::deque<std::vector<uint8_t>>& CallLog::spiInQueue()
{
	static std::deque<std::vector<uint8_t>> queue;
	return queue;
}

std::deque<uint8_t>& CallLog::uartRxQueue()
{
	static std::deque<uint8_t> queue;
	return queue;
}

void CallLog::reset()
{
	mutableCalls().clear();
	readQueue().clear();
	spiInQueue().clear();
	uartRxQueue().clear();
	g_isReadyStatus = ISL_StatusTypeDef::ISL_OK;
	g_tick = 0;
	g_gpioLevel = 0;
	g_gpioValid = true;
}

const std::vector<BusCall>& CallLog::calls()
{
	return mutableCalls();
}

void CallLog::record(BusCall call)
{
	mutableCalls().push_back(std::move(call));
}

void CallLog::setIsReadyStatus(ISL_StatusTypeDef status)
{
	g_isReadyStatus = status;
}

ISL_StatusTypeDef CallLog::takeIsReadyStatus()
{
	return g_isReadyStatus;
}

void CallLog::enqueueRead(std::vector<uint8_t> bytes)
{
	readQueue().push_back(std::move(bytes));
}

void CallLog::fillRead(uint8_t* dest, uint8_t nBytes)
{
	if (dest == nullptr || nBytes == 0) {
		return;
	}
	std::fill(dest, dest + nBytes, 0);
	if (readQueue().empty()) {
		return;
	}
	std::vector<uint8_t> bytes = std::move(readQueue().front());
	readQueue().pop_front();
	const size_t copyCount = std::min(static_cast<size_t>(nBytes), bytes.size());
	if (copyCount > 0) {
		std::memcpy(dest, bytes.data(), copyCount);
	}
}

void CallLog::setTick(uint32_t tickValue)
{
	g_tick = tickValue;
}

uint32_t CallLog::tick()
{
	return g_tick;
}

void CallLog::advanceTick(uint32_t delta)
{
	g_tick += delta;
}

void CallLog::setGpioLevel(uint8_t level)
{
	g_gpioLevel = level ? 1 : 0;
}

uint8_t CallLog::gpioLevel()
{
	return g_gpioLevel;
}

void CallLog::setGpioValid(bool valid)
{
	g_gpioValid = valid;
}

bool CallLog::gpioValid()
{
	return g_gpioValid;
}

void CallLog::enqueueSpiIn(std::vector<uint8_t> bytes)
{
	spiInQueue().push_back(std::move(bytes));
}

void CallLog::fillSpiIn(uint8_t* dest, uint8_t nBytes)
{
	if (dest == nullptr || nBytes == 0) {
		return;
	}
	std::fill(dest, dest + nBytes, 0);
	if (spiInQueue().empty()) {
		return;
	}
	std::vector<uint8_t> bytes = std::move(spiInQueue().front());
	spiInQueue().pop_front();
	const size_t copyCount = std::min(static_cast<size_t>(nBytes), bytes.size());
	if (copyCount > 0) {
		std::memcpy(dest, bytes.data(), copyCount);
	}
}

void CallLog::enqueueUartRx(std::vector<uint8_t> bytes)
{
	for (uint8_t byte : bytes) {
		uartRxQueue().push_back(byte);
	}
}

uint8_t CallLog::uartAvailable()
{
	const size_t size = uartRxQueue().size();
	return size > 255 ? 255 : static_cast<uint8_t>(size);
}

ISL_StatusTypeDef CallLog::takeUartRx(uint8_t* dest, uint16_t count)
{
	if (dest == nullptr) {
		return ISL_StatusTypeDef::ISL_ERROR;
	}
	for (uint16_t i = 0; i < count; ++i) {
		if (uartRxQueue().empty()) {
			return ISL_StatusTypeDef::ISL_TIMEOUT;
		}
		dest[i] = uartRxQueue().front();
		uartRxQueue().pop_front();
	}
	return ISL_StatusTypeDef::ISL_OK;
}

} /* namespace test */
} /* namespace IntroSatLib */
