/**
 * @file FakeI2C.cpp
 * @brief Recording fake for IntroSatLib::interfaces::I2C (host unit tests).
 */
#define ISL_INTERNAL
#include "Adapter/I2C.h"

#if defined(ISL_I2C_ENABLED)

#include "CallLog.h"
#include "Logger.h"

#include <cstring>
#include <vector>

using IntroSatLib::test::BusCall;
using IntroSatLib::test::BusOp;
using IntroSatLib::test::CallLog;

IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE* hi2c) : _hi2c(hi2c) {}

IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE* hi2c, I2CSpeed speed)
	: _hi2c(hi2c), _speed(speed)
{
}

IntroSatLib::interfaces::I2C::I2C(const I2C& other)
	: _hi2c(other._hi2c), _speed(other._speed)
{
}

IntroSatLib::interfaces::I2C::I2C(I2C&& other)
	: _hi2c(other._hi2c), _speed(other._speed)
{
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(const I2C& other)
{
	if (this != &other) {
		_hi2c = other._hi2c;
		_speed = other._speed;
	}
	return *this;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(I2C&& other)
{
	if (this != &other) {
		_hi2c = other._hi2c;
		_speed = other._speed;
	}
	return *this;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::innerIsReady(uint8_t address)
{
	CallLog::record(BusCall{
		BusOp::I2C_IsReady,
		address,
		0,
		0,
		0,
		{},
	});
	return CallLog::takeIsReadyStatus();
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::isReady(uint8_t deviceAddress, uint8_t waitIsReady)
{
	ASSERT_I2C_HAVE();
	while (true) {
		ISL_StatusTypeDef status = innerIsReady(deviceAddress);
		if (status == ISL_StatusTypeDef::ISL_OK) {
			return ISL_StatusTypeDef::ISL_OK;
		}
		if (waitIsReady == 0) {
			return status;
		}
	}
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::read(
	uint8_t deviceAddress,
	uint8_t* data,
	uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	CallLog::fillRead(data, nBytes);
	std::vector<uint8_t> captured;
	if (data != nullptr && nBytes > 0) {
		captured.assign(data, data + nBytes);
	}
	CallLog::record(BusCall{
		BusOp::I2C_Read,
		deviceAddress,
		0,
		nBytes,
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::readMem(
	uint8_t deviceAddress,
	uint8_t reg,
	uint8_t* data,
	uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	CallLog::fillRead(data, nBytes);
	std::vector<uint8_t> captured;
	if (data != nullptr && nBytes > 0) {
		captured.assign(data, data + nBytes);
	}
	CallLog::record(BusCall{
		BusOp::I2C_ReadMem,
		deviceAddress,
		reg,
		nBytes,
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::write(
	uint8_t deviceAddress,
	uint8_t* data,
	uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	std::vector<uint8_t> captured;
	if (data != nullptr && nBytes > 0) {
		captured.assign(data, data + nBytes);
	}
	CallLog::record(BusCall{
		BusOp::I2C_Write,
		deviceAddress,
		0,
		nBytes,
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::writeMem(
	uint8_t deviceAddress,
	uint8_t reg,
	uint8_t* data,
	uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	std::vector<uint8_t> captured;
	if (data != nullptr && nBytes > 0) {
		captured.assign(data, data + nBytes);
	}
	CallLog::record(BusCall{
		BusOp::I2C_WriteMem,
		deviceAddress,
		reg,
		nBytes,
		0,
		std::move(captured),
	});
	return ISL_StatusTypeDef::ISL_OK;
}

IntroSatLib::interfaces::I2C::~I2C() = default;

#endif /* ISL_I2C_ENABLED */
