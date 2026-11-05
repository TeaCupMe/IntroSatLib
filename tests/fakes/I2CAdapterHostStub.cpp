/**
 * @file I2CAdapterHostStub.cpp
 * @brief Minimal I2CAdapter link stub for host unit tests (MockI2C DI path).
 *
 * Not a platform backend. Provides symbols so drivers that reference handle
 * constructors / I2CAdapter::from can link; handle/pool behavior is untested.
 * Driver bus tests inject MockI2C instead.
 */
#define ISL_INTERNAL
#include "Adapter/I2C/I2CAdapter.h"

#if defined(ISL_I2C_ENABLED)

#include <cstdint>

IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2C_HANDLE_TYPE* hi2c) : _hi2c(hi2c) {}

IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2C_HANDLE_TYPE* hi2c, I2CSpeed speed)
	: _hi2c(hi2c), _speed(speed)
{
}

IntroSatLib::interfaces::I2CAdapter::I2CAdapter(const I2CAdapter& other)
	: _hi2c(other._hi2c), _speed(other._speed)
{
}

IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2CAdapter&& other)
	: _hi2c(other._hi2c), _speed(other._speed)
{
}

IntroSatLib::interfaces::I2CAdapter& IntroSatLib::interfaces::I2CAdapter::operator=(const I2CAdapter& other)
{
	if (this != &other) {
		_hi2c = other._hi2c;
		_speed = other._speed;
	}
	return *this;
}

IntroSatLib::interfaces::I2CAdapter& IntroSatLib::interfaces::I2CAdapter::operator=(I2CAdapter&& other)
{
	if (this != &other) {
		_hi2c = other._hi2c;
		_speed = other._speed;
	}
	return *this;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::innerIsReady(uint8_t /*address*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::isReady(
	uint8_t /*deviceAddress*/,
	uint8_t /*waitIsReady*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::read(
	uint8_t /*deviceAddress*/,
	uint8_t* /*data*/,
	uint8_t /*nBytes*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::readMem(
	uint8_t /*deviceAddress*/,
	uint8_t /*reg*/,
	uint8_t* /*data*/,
	uint8_t /*nBytes*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::write(
	uint8_t /*deviceAddress*/,
	uint8_t* /*data*/,
	uint8_t /*nBytes*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::writeMem(
	uint8_t /*deviceAddress*/,
	uint8_t /*reg*/,
	uint8_t* /*data*/,
	uint8_t /*nBytes*/)
{
	return ISL_StatusTypeDef::ISL_ERROR;
}

IntroSatLib::interfaces::I2CAdapter::~I2CAdapter() = default;

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2CAdapter::from(I2C_HANDLE_TYPE* /*hi2c*/)
{
	static I2CAdapter unused{nullptr};
	return unused;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2CAdapter::from(I2C_HANDLE_TYPE& hi2c)
{
	return from(&hi2c);
}

#endif /* ISL_I2C_ENABLED */
