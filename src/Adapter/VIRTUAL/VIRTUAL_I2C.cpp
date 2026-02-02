#define ISL_INTERNAL
#include "Adapter/I2C.h"

#ifdef ISL_VIRTUAL

#include "Logger.h"


IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE *hi2c): _hi2c(hi2c)
{
}

IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed): _hi2c(hi2c), _speed(speed)
{
}

IntroSatLib::interfaces::I2C::I2C(const I2C& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

IntroSatLib::interfaces::I2C::I2C(I2C&& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(const I2C& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(I2C&& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::isReady(uint8_t deviceAddress, uint8_t waitIsReady)
{
	ASSERT_I2C_HAVE();
	while(true)
	{
		ISL_StatusTypeDef status = innerIsReady(deviceAddress);
		if (status == ISL_StatusTypeDef::ISL_OK) { return ISL_StatusTypeDef::ISL_OK; }
		if (waitIsReady == 0) { return status; }
	}
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::innerIsReady(uint8_t deviceAddress)
{
	LOG_I2C_ADDRESS();
	logText(": ");
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_ERROR;
	if (_hi2c->IsReady(deviceAddress)) status = ISL_OK;
	else status = ISL_ERROR;
	logStatus(status);
	logText("\n");
	return status;
}


ISL_StatusTypeDef IntroSatLib::interfaces::I2C::read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read ");
	logNumber(nBytes);

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	
	uint8_t rxCount = _hi2c->Receive(deviceAddress, data, nBytes);

	if (rxCount < nBytes) { status = logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }

	if (status == ISL_StatusTypeDef::ISL_OK) {
		logStatus(status);

		logText(" bytes > ");
		LOG_I2C_BUFFER(", ", data, nBytes);
	}

	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read from memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	// status |= 
	// status |= _hi2c->Transmit(deviceAddress, &reg, 1);
	status = _hi2c->Transmit(deviceAddress, &reg, 1) ? ISL_OK : ISL_ERROR;

	uint8_t rxCount = _hi2c->Receive(deviceAddress, data, nBytes);

	if (rxCount < nBytes) { status = logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }

	if (status == ISL_StatusTypeDef::ISL_OK) {
		logStatus(status);

		logText(" bytes > ");
		LOG_I2C_BUFFER(", ", data, nBytes);
	}

	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, Nbytes);
	logText(" > ");

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	status = _hi2c->Transmit(deviceAddress, data, nBytes) ? ISL_OK : ISL_ERROR;

	logStatus(status);
	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write to memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, nBytes);
	logText(" > ");

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	status = _hi2c->Transmit(deviceAddress, &reg, 1) ? ISL_OK : ISL_ERROR;
	status = _hi2c->Transmit(deviceAddress, data, nBytes) ? ISL_OK : ISL_ERROR;

	logStatus(status);
	logText("\n");
	return status;
}



IntroSatLib::interfaces::I2C::~I2C()
{
}

#endif /* ISL_VIRTUAL */
