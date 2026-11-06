/**
 * @file MockI2C.hpp
 * @brief Unified DI MockI2C for driver unit tests.
 *
 * Program the expected call sequence (ops + payloads + RX/status) up front via
 * expect(), record actual calls while serving programmed responses, then assert
 * with REQUIRE_I2C_CALLS(bus). No mid-call failure.
 *
 * Optional full call logging (silent by default):
 *   bus.setLogging(true);
 *   // or: export INTROSATLIB_MOCK_I2C_LOG=1
 * Pretty lines via HostLog (timestamp, DEBUG level, [MockI2C] metadata, color).
 */
#ifndef INTROSATLIB_TEST_MOCK_I2C_HPP_
#define INTROSATLIB_TEST_MOCK_I2C_HPP_

#include "Adapter/I2C/I2C.h"
#include "HostLog.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <string>
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
	MockI2C() { _logging = loggingEnabledFromEnv(); }

	void reset()
	{
		_expected.clear();
		_calls.clear();
	}

	/** Optional label shown in log metadata (default "bus"). */
	void setBusName(const char* name) { _busName = name != nullptr ? name : "bus"; }
	const std::string& busName() const { return _busName; }

	/** Enable/disable per-call logging (default off unless env set). */
	void setLogging(bool enabled) { _logging = enabled; }
	bool logging() const { return _logging; }

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

	/** Log the full recorded sequence (HostLog DEBUG; no-op if logging off). */
	void dumpCalls() const
	{
		if (!_logging) {
			return;
		}
		HostLog::info(
			"MockI2C",
			"dump bus=%s calls=%zu",
			_busName.c_str(),
			_calls.size());
		for (size_t i = 0; i < _calls.size(); ++i) {
			const ISL_StatusTypeDef status =
				(i < _expected.size()) ? _expected[i].returnStatus : ISL_StatusTypeDef::ISL_OK;
			logOne(i, _calls[i], status);
		}
	}

	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t /*waitIsReady*/ = 0) override
	{
		const size_t idx = _calls.size();
		I2CCall call{I2COp::IsReady, deviceAddress, 0, 0, {}};
		_calls.push_back(call);
		const ISL_StatusTypeDef status = statusAt(idx);
		logOne(idx, call, status);
		return status;
	}

	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		fillRead(idx, data, nBytes);
		I2CCall call{
			I2COp::Read,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		};
		_calls.push_back(call);
		const ISL_StatusTypeDef status = statusAt(idx);
		logOne(idx, call, status);
		return status;
	}

	ISL_StatusTypeDef readMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		fillRead(idx, data, nBytes);
		I2CCall call{
			I2COp::ReadMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		};
		_calls.push_back(call);
		const ISL_StatusTypeDef status = statusAt(idx);
		logOne(idx, call, status);
		return status;
	}

	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		I2CCall call{
			I2COp::Write,
			deviceAddress,
			0,
			nBytes,
			capture(data, nBytes),
		};
		_calls.push_back(call);
		const ISL_StatusTypeDef status = statusAt(idx);
		logOne(idx, call, status);
		return status;
	}

	ISL_StatusTypeDef writeMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) override
	{
		const size_t idx = _calls.size();
		I2CCall call{
			I2COp::WriteMem,
			deviceAddress,
			reg,
			nBytes,
			capture(data, nBytes),
		};
		_calls.push_back(call);
		const ISL_StatusTypeDef status = statusAt(idx);
		logOne(idx, call, status);
		return status;
	}

private:
	static bool loggingEnabledFromEnv()
	{
		const char* value = std::getenv("INTROSATLIB_MOCK_I2C_LOG");
		if (value == nullptr || value[0] == '\0') {
			return false;
		}
		return !(value[0] == '0' && value[1] == '\0');
	}

	static const char* opName(I2COp op)
	{
		switch (op) {
		case I2COp::IsReady:
			return "IsReady";
		case I2COp::Read:
			return "Read";
		case I2COp::Write:
			return "Write";
		case I2COp::ReadMem:
			return "ReadMem";
		case I2COp::WriteMem:
			return "WriteMem";
		}
		return "?";
	}

	static const char* statusName(ISL_StatusTypeDef status)
	{
		switch (status) {
		case ISL_StatusTypeDef::ISL_OK:
			return "OK";
		case ISL_StatusTypeDef::ISL_ERROR:
			return "ERROR";
		case ISL_StatusTypeDef::ISL_BUSY:
			return "BUSY";
		case ISL_StatusTypeDef::ISL_TIMEOUT:
			return "TIMEOUT";
		default:
			return "UNKNOWN";
		}
	}

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

	static std::string formatBytes(const std::vector<uint8_t>& bytes)
	{
		std::string out = "[";
		char hex[8];
		for (size_t i = 0; i < bytes.size(); ++i) {
			if (i != 0) {
				out += ',';
			}
			std::snprintf(hex, sizeof(hex), "0x%02X", bytes[i]);
			out += hex;
		}
		out += ']';
		return out;
	}

	void logOne(size_t idx, const I2CCall& call, ISL_StatusTypeDef status) const
	{
		if (!_logging) {
			return;
		}

		char message[512];
		if (call.op == I2COp::IsReady) {
			std::snprintf(
				message,
				sizeof(message),
				"bus=%s idx=%zu op=%s addr=0x%02X -> %s",
				_busName.c_str(),
				idx,
				opName(call.op),
				call.address,
				statusName(status));
		} else if (call.op == I2COp::ReadMem || call.op == I2COp::WriteMem) {
			const std::string data = formatBytes(call.data);
			std::snprintf(
				message,
				sizeof(message),
				"bus=%s idx=%zu op=%s addr=0x%02X reg=0x%02X n=%u data=%s -> %s",
				_busName.c_str(),
				idx,
				opName(call.op),
				call.address,
				call.reg,
				call.nBytes,
				data.c_str(),
				statusName(status));
		} else {
			const std::string data = formatBytes(call.data);
			std::snprintf(
				message,
				sizeof(message),
				"bus=%s idx=%zu op=%s addr=0x%02X n=%u data=%s -> %s",
				_busName.c_str(),
				idx,
				opName(call.op),
				call.address,
				call.nBytes,
				data.c_str(),
				statusName(status));
		}

		const LogLevel level =
			(status == ISL_StatusTypeDef::ISL_OK) ? LogLevel::Debug : LogLevel::Warn;
		HostLog::log(level, "MockI2C", "%s", message);
	}

	std::vector<ExpectedI2CCall> _expected;
	std::vector<I2CCall> _calls;
	bool _logging = false;
	std::string _busName = "bus";
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_MOCK_I2C_HPP_ */
