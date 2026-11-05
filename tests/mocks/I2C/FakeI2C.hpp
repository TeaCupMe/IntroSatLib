/**
 * @file FakeI2C.hpp
 * @brief Recording FakeI2C for DI-based driver unit tests.
 *
 * Prefer this header + I2CCallHelpers.hpp for driver call/args asserts.
 * Link-time Fake*.cpp under tests/fakes/ exist only so host builds of Core
 * can replace platform Adapter/System objects — not for pilot assertions.
 */
#ifndef INTROSATLIB_TEST_FAKE_I2C_HPP_
#define INTROSATLIB_TEST_FAKE_I2C_HPP_

#include "Adapter/I2C/I2C.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <deque>
#include <vector>

namespace IntroSatLib {
namespace test {

enum class I2COp : uint8_t {
	IsReady,
	Read,
	Write,
	ReadMem,
	WriteMem,
};

struct I2CCall {
	I2COp op{};
	uint8_t address = 0;
	uint8_t reg = 0;
	uint8_t nBytes = 0;
	std::vector<uint8_t> data;
};

/**
 * Header-only recording spy implementing interfaces::I2C.
 * Inject into drivers via I2CDevice(I2C&) / LM75A(bus) / LSM6DS3(bus).
 */
class FakeI2C final : public interfaces::I2C {
public:
	void reset()
	{
		_calls.clear();
		_readQueue.clear();
		_isReadyStatus = ISL_StatusTypeDef::ISL_OK;
	}

	const std::vector<I2CCall>& calls() const { return _calls; }

	void setIsReadyStatus(ISL_StatusTypeDef status) { _isReadyStatus = status; }

	void enqueueRead(std::vector<uint8_t> bytes) { _readQueue.push_back(std::move(bytes)); }

	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t /*waitIsReady*/ = 0) override
	{
		_calls.push_back(I2CCall{I2COp::IsReady, deviceAddress, 0, 0, {}});
		return _isReadyStatus;
	}

	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		fillRead(data, nBytes);
		_calls.push_back(I2CCall{
			I2COp::Read,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		});
		return ISL_StatusTypeDef::ISL_OK;
	}

	ISL_StatusTypeDef readMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		fillRead(data, nBytes);
		_calls.push_back(I2CCall{
			I2COp::ReadMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		});
		return ISL_StatusTypeDef::ISL_OK;
	}

	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		_calls.push_back(I2CCall{
			I2COp::Write,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		});
		return ISL_StatusTypeDef::ISL_OK;
	}

	ISL_StatusTypeDef writeMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		_calls.push_back(I2CCall{
			I2COp::WriteMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		});
		return ISL_StatusTypeDef::ISL_OK;
	}

private:
	static std::vector<uint8_t> capture(const uint8_t* data, uint8_t nBytes)
	{
		if (data == nullptr || nBytes == 0) {
			return {};
		}
		return std::vector<uint8_t>(data, data + nBytes);
	}

	void fillRead(uint8_t* dest, uint8_t nBytes)
	{
		if (dest == nullptr || nBytes == 0) {
			return;
		}
		std::fill(dest, dest + nBytes, 0);
		if (_readQueue.empty()) {
			return;
		}
		std::vector<uint8_t> bytes = std::move(_readQueue.front());
		_readQueue.pop_front();
		const size_t copyCount = std::min(static_cast<size_t>(nBytes), bytes.size());
		if (copyCount > 0) {
			std::memcpy(dest, bytes.data(), copyCount);
		}
	}

	std::vector<I2CCall> _calls;
	std::deque<std::vector<uint8_t>> _readQueue;
	ISL_StatusTypeDef _isReadyStatus = ISL_StatusTypeDef::ISL_OK;
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_FAKE_I2C_HPP_ */
