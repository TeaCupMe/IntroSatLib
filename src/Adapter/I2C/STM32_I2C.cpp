
#define ISL_INTERNAL
#include "Adapter/I2C/I2CAdapter.h"

#if defined(HAL_I2C_MODULE_ENABLED) && !defined(ARDUINO)

#include <Adapter/STM32/STM32_I2C_Err.h>
#include "Logger.h"




IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2C_HANDLE_TYPE *hi2c): _hi2c(hi2c)
{
}

IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed): _hi2c(hi2c), _speed(speed)
{
}

//#ifdef ARDUINO
//IntroSatLib::interfaces::I2CAdapter::I2CAdapter(TwoWire &hi2c): _hi2c(hi2c.getHandle())
//{
//}
//
//IntroSatLib::interfaces::I2CAdapter::I2CAdapter(TwoWire &hi2c, I2CSpeed speed): _hi2c(hi2c.getHandle()), _speed(speed)
//{
//}
//#endif

// TODO Move to Header
IntroSatLib::interfaces::I2CAdapter::I2CAdapter(const I2CAdapter& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

// TODO Move to Header
IntroSatLib::interfaces::I2CAdapter::I2CAdapter(I2CAdapter&& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

// TODO Move to Header
IntroSatLib::interfaces::I2CAdapter& IntroSatLib::interfaces::I2CAdapter::operator=(const I2CAdapter& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

// TODO Move to Header
IntroSatLib::interfaces::I2CAdapter& IntroSatLib::interfaces::I2CAdapter::operator=(I2CAdapter&& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

// TODO Move to Header
ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::isReady(uint8_t deviceAddress, uint8_t waitIsReady)
{
	ASSERT_I2C_HAVE();
	while(true)
	{
		ISL_StatusTypeDef status = innerIsReady(deviceAddress);
		if (status == ISL_StatusTypeDef::ISL_OK) { return ISL_StatusTypeDef::ISL_OK; }
		if (waitIsReady == 0) { return status; }
	}
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::innerIsReady(uint8_t deviceAddress)
{
	LOG_I2C_ADDRESS();
	logText(": ");
	ISL_StatusTypeDef status = logStatus(
			(ISL_StatusTypeDef) HAL_I2C_IsDeviceReady(_hi2c, deviceAddress << 1, 1, 1000)
	);
	logText("\n");
	return status;
}


ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read ");
	logNumber(nBytes);
	logText("bytes > ");

	ISL_StatusTypeDef status = logStatus(
			static_cast<ISL_StatusTypeDef>(HAL_I2C_Master_Receive(_hi2c, deviceAddress << 1, data, nBytes, 1000))
	);
	if (status == ISL_StatusTypeDef::ISL_OK) { LOG_I2C_BUFFER(", ", data, nBytes); }
	else {I2C_ErrorAnalyzer(_hi2c);}

	logText("\n");
	return status;

}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read from memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);
	logText("bytes > ");
	ISL_StatusTypeDef status = logStatus(
			(ISL_StatusTypeDef) HAL_I2C_Mem_Read(
			_hi2c,
			deviceAddress << 1,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			nBytes,
			1000
		)
	);

	if (status == ISL_StatusTypeDef::ISL_OK) { LOG_I2C_BUFFER(", ", data, nBytes); }
	else {I2C_ErrorAnalyzer(_hi2c);}

	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, nBytes);
	logText(" > ");
	ISL_StatusTypeDef status = logStatus(
			(ISL_StatusTypeDef) HAL_I2C_Master_Transmit(_hi2c, deviceAddress << 1, data, nBytes, 1000)
	);

	if (status == ISL_StatusTypeDef::ISL_OK) { LOG_I2C_BUFFER(", ", data, nBytes); }
	else {I2C_ErrorAnalyzer(_hi2c);}

	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2CAdapter::writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write from memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, nBytes);
	logText(" > ");
	ISL_StatusTypeDef status = logStatus(
			(ISL_StatusTypeDef) HAL_I2C_Mem_Write(
				_hi2c,
				deviceAddress << 1,
				reg,
				I2C_MEMADD_SIZE_8BIT,
				data,
				nBytes,
				1000)
	);
	if (status == ISL_StatusTypeDef::ISL_OK) { LOG_I2C_BUFFER(", ", data, nBytes); }
	else {I2C_ErrorAnalyzer(_hi2c);}

	logText("\n");
	return status;
}


// TODO Move to Header
IntroSatLib::interfaces::I2CAdapter::~I2CAdapter()
{
}

#endif /* HAL_I2C_MODULE_ENABLED */
