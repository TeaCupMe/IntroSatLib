/**
 * @file FakeUART.cpp
 * @brief Recording fake for IntroSatLib::interfaces::UART (host unit tests).
 */
#define ISL_INTERNAL
#include "Adapter/UART.h"

#if defined(ISL_UART_ENABLED)

#include "CallLog.h"
#include "Logger.h"

#include <vector>

using IntroSatLib::test::BusCall;
using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

uint8_t IntroSatLib::interfaces::UART::available()
{
	const uint8_t count = CallLog::uartAvailable();
	CallLog::record(BusCall{BusOp::UART_Available, 0, 0, count, 0, {}});
	return count;
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::receive(
	uint8_t* rx_buf,
	uint16_t count,
	uint16_t /*timeout*/)
{
	ASSERT_UART_HAVE();
	const ISL_StatusTypeDef status = CallLog::takeUartRx(rx_buf, count);
	std::vector<uint8_t> captured;
	if (status == ISL_StatusTypeDef::ISL_OK && rx_buf != nullptr && count > 0) {
		captured.assign(rx_buf, rx_buf + count);
	}
	CallLog::record(BusCall{
		BusOp::UART_Receive,
		0,
		0,
		static_cast<uint8_t>(count > 255 ? 255 : count),
		0,
		std::move(captured),
	});
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmit(
	uint8_t* tx_buf,
	uint16_t count,
	uint16_t /*timeout*/)
{
	ASSERT_UART_HAVE();
	std::vector<uint8_t> captured;
	if (tx_buf != nullptr && count > 0) {
		captured.assign(tx_buf, tx_buf + count);
	}
	CallLog::record(BusCall{
		BusOp::UART_Transmit,
		0,
		0,
		static_cast<uint8_t>(count > 255 ? 255 : count),
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmitAsync(
	uint8_t* tx_buf,
	uint16_t count)
{
	ASSERT_UART_HAVE();
	std::vector<uint8_t> captured;
	if (tx_buf != nullptr && count > 0) {
		captured.assign(tx_buf, tx_buf + count);
	}
	CallLog::record(BusCall{
		BusOp::UART_TransmitAsync,
		0,
		0,
		static_cast<uint8_t>(count > 255 ? 255 : count),
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

#endif /* ISL_UART_ENABLED */
