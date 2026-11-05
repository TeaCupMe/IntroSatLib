/**
 * @file I2CCallHelpers.hpp
 * @brief Compact Catch2 assertions for MockI2C call sequences.
 */
#ifndef INTROSATLIB_TEST_I2C_CALL_HELPERS_HPP_
#define INTROSATLIB_TEST_I2C_CALL_HELPERS_HPP_

#include "CallLog.h"
#include "MockI2C.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace IntroSatLib {
namespace test {

inline ExpectedI2CCall IsReady(uint8_t address)
{
	ExpectedI2CCall call;
	call.op = I2COp::IsReady;
	call.address = address;
	call.matchAddress = true;
	return call;
}

inline ExpectedI2CCall ReadMem(uint8_t address, uint8_t reg, uint8_t nBytes)
{
	ExpectedI2CCall call;
	call.op = I2COp::ReadMem;
	call.address = address;
	call.reg = reg;
	call.nBytes = nBytes;
	call.matchAddress = true;
	call.matchReg = true;
	call.matchNBytes = true;
	return call;
}

inline ExpectedI2CCall WriteMem(uint8_t address, uint8_t reg, std::vector<uint8_t> data)
{
	ExpectedI2CCall call;
	call.op = I2COp::WriteMem;
	call.address = address;
	call.reg = reg;
	call.nBytes = static_cast<uint8_t>(data.size());
	call.data = std::move(data);
	call.matchAddress = true;
	call.matchReg = true;
	call.matchNBytes = true;
	call.matchData = true;
	return call;
}

inline ExpectedI2CCall Read(uint8_t address, uint8_t nBytes)
{
	ExpectedI2CCall call;
	call.op = I2COp::Read;
	call.address = address;
	call.nBytes = nBytes;
	call.matchAddress = true;
	call.matchNBytes = true;
	return call;
}

inline ExpectedI2CCall Write(uint8_t address, std::vector<uint8_t> data)
{
	ExpectedI2CCall call;
	call.op = I2COp::Write;
	call.address = address;
	call.nBytes = static_cast<uint8_t>(data.size());
	call.data = std::move(data);
	call.matchAddress = true;
	call.matchNBytes = true;
	call.matchData = true;
	return call;
}

inline const char* i2cOpName(I2COp op)
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

inline std::string formatBytes(const std::vector<uint8_t>& bytes)
{
	std::ostringstream out;
	out << '[';
	for (size_t i = 0; i < bytes.size(); ++i) {
		if (i != 0) {
			out << ',';
		}
		out << static_cast<unsigned>(bytes[i]);
	}
	out << ']';
	return out.str();
}

inline void requireI2CCalls(
	const std::vector<I2CCall>& actual,
	const std::vector<ExpectedI2CCall>& expected)
{
	REQUIRE(actual.size() == expected.size());
	for (size_t i = 0; i < expected.size(); ++i) {
		INFO("I2C call index " << i << " expected " << i2cOpName(expected[i].op)
							   << " actual " << i2cOpName(actual[i].op));
		REQUIRE(actual[i].op == expected[i].op);
		if (expected[i].matchAddress) {
			REQUIRE(actual[i].address == expected[i].address);
		}
		if (expected[i].matchReg) {
			REQUIRE(actual[i].reg == expected[i].reg);
		}
		if (expected[i].matchNBytes) {
			REQUIRE(actual[i].nBytes == expected[i].nBytes);
		}
		if (expected[i].matchData) {
			INFO("expected data " << formatBytes(expected[i].data)
								  << " actual data " << formatBytes(actual[i].data));
			REQUIRE(actual[i].data == expected[i].data);
		}
	}
}

inline void requireI2CCalls(const MockI2C& bus)
{
	requireI2CCalls(bus.calls(), bus.expected());
}

inline void requireSystemDelay(uint32_t milliseconds)
{
	const auto& calls = CallLog::calls();
	REQUIRE_FALSE(calls.empty());
	bool found = false;
	for (const auto& call : calls) {
		if (call.op == BusOp::System_Delay && call.value == milliseconds) {
			found = true;
			break;
		}
	}
	REQUIRE(found);
}

} /* namespace test */
} /* namespace IntroSatLib */

/**
 * Assert MockI2C recorded calls match the sequence programmed via bus.expect(...).
 *
 * Example:
 *   bus.expect({
 *       IsReady(0x48),
 *       ReadMem(0x48, 0x00, 2).respond({0x19, 0x00}),
 *       WriteMem(0x6B, 0x11, {0x04}),
 *   });
 *   // ... exercise driver ...
 *   REQUIRE_I2C_CALLS(bus);
 */
#define REQUIRE_I2C_CALLS(bus)                                                           \
	do {                                                                                 \
		::IntroSatLib::test::requireI2CCalls(bus);                                       \
	} while (0)

/** Assert CallLog saw system::Delay(ms) (VIRTUAL host System stub). */
#define REQUIRE_SYSTEM_DELAY(ms)                                                         \
	do {                                                                                 \
		::IntroSatLib::test::requireSystemDelay(static_cast<uint32_t>(ms));              \
	} while (0)

#endif /* INTROSATLIB_TEST_I2C_CALL_HELPERS_HPP_ */
