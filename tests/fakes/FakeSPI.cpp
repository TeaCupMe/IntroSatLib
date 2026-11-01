/**
 * @file FakeSPI.cpp
 * @brief Recording fake for IntroSatLib::interfaces::SPI (host unit tests).
 */
#define ISL_INTERNAL
#include <cstddef>
#include "Adapter/SPI.h"

#if defined(ISL_SPI_ENABLED)

#include "CallLog.h"

#include <vector>

using IntroSatLib::test::BusCall;
using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::transfer(
	const uint8_t* out,
	uint8_t* in,
	uint8_t len)
{
	CallLog::fillSpiIn(in, len);
	std::vector<uint8_t> captured;
	if (out != nullptr && len > 0) {
		captured.assign(out, out + len);
	}
	CallLog::record(BusCall{
		BusOp::SPI_Transfer,
		0,
		0,
		len,
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

#endif /* ISL_SPI_ENABLED */
