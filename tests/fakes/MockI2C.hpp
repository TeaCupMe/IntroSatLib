/**
 * @file MockI2C.hpp
 * @brief Unified DI MockI2C for driver unit tests.
 *
 * Program the expected call sequence (ops + payloads + RX/status) up front via
 * expect(), record actual calls while serving programmed responses, then assert
 * with REQUIRE_I2C_CALLS(bus). No mid-call failure.
 *
 * Host builds may link I2CAdapterHostStub.cpp for unused handle symbols only.
 */
#ifndef INTROSATLIB_TEST_MOCK_I2C_HPP_
#define INTROSATLIB_TEST_MOCK_I2C_HPP_

#include "Adapter/I2C/I2C.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <utility>
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
 * One expected bus operation. Use builders in I2CCallHelpers.hpp and chain
 * .respond(...) / .status(...) when the slot needs RX or a non-OK return.
 */
struct ExpectedI2CCall {
	I2COp op{};
	uint8_t address = 0;
	uint8_t reg = 0;
	uint8_t nBytes = 0;
	std::vector<uint8_t> data{};
	std::vector<uint8_t> responseData{};
	ISL_StatusTypeDef returnStatus = ISL_StatusTypeDef::ISL_OK;
	bool matchAddress = true;
	bool matchReg = false;
	bool matchNBytes = false;
	bool matchData = false;

	ExpectedI2CCall& respond(std::initializer_list<uint8_t> bytes)
	{
		responseData.assign(bytes);
		return *this;
	}

	ExpectedI2CCall& respond(std::vector<uint8_t> bytes)
	{
		responseData = std::move(bytes);
		return *this;
	}

	ExpectedI2CCall& status(ISL_StatusTypeDef status)
	{
		returnStatus = status;
		return *this;
	}
};

/**
 * Header-only recording mock implementing interfaces::I2C.
 * Inject into drivers via I2CDevice(I2C&) / LM75A(bus) / LSM6DS3(bus).
 */
class MockI2C final : public interfaces::I2C {
public:
	void reset()
	{
		_expected.clear();
		_calls.clear();
	}

	void expect(std::initializer_list<ExpectedI2CCall> calls)
	{
		_expected.assign(calls);
		_calls.clear();
	}

	void expect(std::vector<ExpectedI2CCall> calls)
	{
		_expected = std::move(calls);
		_calls.clear();
	}

	void enqueue(ExpectedI2CCall call) { _expected.push_back(std::move(call)); }

	const std::vector<ExpectedI2CCall>& expected() const { return _expected; }
	const std::vector<I2CCall>& calls() const { return _calls; }

	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t /*waitIsReady*/ = 0) override
	{
		const size_t idx = _calls.size();
		_calls.push_back(I2CCall{I2COp::IsReady, deviceAddress, 0, 0, {}});
		return statusAt(idx);
	}

	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		fillRead(idx, data, nBytes);
		_calls.push_back(I2CCall{
			I2COp::Read,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		});
		return statusAt(idx);
	}

	ISL_StatusTypeDef readMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		fillRead(idx, data, nBytes);
		_calls.push_back(I2CCall{
			I2COp::ReadMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		});
		return statusAt(idx);
	}

	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		_calls.push_back(I2CCall{
			I2COp::Write,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		});
		return statusAt(idx);
	}

	ISL_StatusTypeDef writeMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		_calls.push_back(I2CCall{
			I2COp::WriteMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		});
		return statusAt(idx);
	}

private:
	static std::vector<uint8_t> capture(const uint8_t* data, uint8_t nBytes)
	{
		if (data == nullptr || nBytes == 0) {
			return {};
		}
		return std::vector<uint8_t>(data, data + nBytes);
	}

	ISL_StatusTypeDef statusAt(size_t idx) const
	{
		if (idx < _expected.size()) {
			return _expected[idx].returnStatus;
		}
		return ISL_StatusTypeDef::ISL_OK;
	}

	void fillRead(size_t idx, uint8_t* dest, uint8_t nBytes)
	{
		if (dest == nullptr || nBytes == 0) {
			return;
		}
		std::fill(dest, dest + nBytes, 0);
		if (idx >= _expected.size()) {
			return;
		}
		const std::vector<uint8_t>& bytes = _expected[idx].responseData;
		const size_t copyCount = std::min(static_cast<size_t>(nBytes), bytes.size());
		if (copyCount > 0) {
			std::memcpy(dest, bytes.data(), copyCount);
		}
	}

	std::vector<ExpectedI2CCall> _expected;
	std::vector<I2CCall> _calls;
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_MOCK_I2C_HPP_ */
