#include "I2C.h"

#include "../Logger.h"

#include "./I2C_Err.h"


#define ASSERT_I2C_HAVE() \
if(!_i2c) { \
	logText("Haven't i2c handle"); \
	return HAL_StatusTypeDef::HAL_ERROR; \
}

#define LOG_I2C_ADDRESS() \
logText("Device in "); \
logHEX(_address >> 1);

#if LOGDATA
#define LOG_I2C_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_I2C_BUFFER(Sep, Data, Nbytes)
#endif


#ifndef ARDUINO
template <typename T>
IntroSatLib::intefaces::I2C::I2C(T *hi2c, uint8_t address)
{
	_i2c = static_cast<I2C_HandleTypeDef>(hi2c);
	if (address > 127) address = 127;
	_address = address << 1;
}

IntroSatLib::intefaces::I2C::I2C(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed)
{
	_i2c = hi2c;
	if (address > 127) address = 127;
	_address = address << 1;
	_speed = speed;
}
#else
IntroSatLib::intefaces::I2C::I2CDevice(TwoWire &hi2c, uint8_t address)
{
	_i2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
}

IntroSatLib::intefaces::I2C::I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed)
{
	_i2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
	_speed = speed;
}
#endif

IntroSatLib::intefaces::I2C::I2C(const I2C& other)
{
	_address = other._address;
	_i2c = other._i2c;
	_speed = other._speed;
}

IntroSatLib::intefaces::I2C::I2C(I2C&& other)
{
	_address = other._address;
	_i2c = other._i2c;
	_speed = other._speed;
}

IntroSatLib::intefaces::I2C& IntroSatLib::intefaces::I2C::operator=(const I2C& other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_i2c = other._i2c;
	_speed = other._speed;
	return *this;
}

IntroSatLib::intefaces::I2C& IntroSatLib::intefaces::I2C::operator=(I2C&& other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_i2c = other._i2c;
	_speed = other._speed;
	return *this;
}
HAL_StatusTypeDef IntroSatLib::intefaces::I2C::isReady(uint8_t waitIsReady)
{
	ASSERT_I2C_HAVE();
	while(true)
	{
		HAL_StatusTypeDef status = innerIsReady();
		if (status == HAL_OK) { return HAL_OK; }
		if (waitIsReady == 0) { return status; }
	}
}

HAL_StatusTypeDef IntroSatLib::intefaces::I2C::innerIsReady()
{
	LOG_I2C_ADDRESS();
	logText(": ");
	HAL_StatusTypeDef status = logStatus(
		HAL_I2C_IsDeviceReady(_i2c, _address, 1, 1000)
	);
	logText("\n");
	return status;
}


HAL_StatusTypeDef IntroSatLib::intefaces::I2C::read(uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read ");
	logNumber(Nbytes);
	logText("bytes > ");

	HAL_StatusTypeDef status = logStatus(
			HAL_I2C_Master_Receive(_i2c, _address, Data, Nbytes, 1000)
	);
	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }
	else {I2C_ErrorAnalyzer(_i2c);}

	logText("\n");
	return status;

}

HAL_StatusTypeDef IntroSatLib::intefaces::I2C::read(uint8_t Register, uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read from memory ");
	logHEX(Register);
	logText(" ");
	logNumber(Nbytes);
	logText("bytes > ");
	HAL_StatusTypeDef status = logStatus(
		HAL_I2C_Mem_Read(
			_i2c,
			_address,
			Register,
			I2C_MEMADD_SIZE_8BIT,
			Data,
			Nbytes,
			1000
		)
	);

	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }
	else {I2C_ErrorAnalyzer(_i2c);}

	logText("\n");
	return status;
}

HAL_StatusTypeDef IntroSatLib::intefaces::I2C::write(uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write ");
	logNumber(Nbytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", Data, Nbytes);
	logText(" > ");
	HAL_StatusTypeDef status = logStatus(
			HAL_I2C_Master_Transmit(_i2c, _address, Data, Nbytes, 1000)
	);

	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }
	else {I2C_ErrorAnalyzer(_i2c);}

	logText("\n");
	return status;
}

HAL_StatusTypeDef IntroSatLib::intefaces::I2C::write(uint8_t Register, uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write from memory ");
	logHEX(Register);
	logText(" ");
	logNumber(Nbytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", Data, Nbytes);
	logText(" > ");
	HAL_StatusTypeDef status = logStatus(
		HAL_I2C_Mem_Write(
				_i2c,
				_address,
				Register,
				I2C_MEMADD_SIZE_8BIT,
				Data,
				Nbytes,
				1000)
	);
	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }
	else {I2C_ErrorAnalyzer(_i2c);}

	logText("\n");
	return status;
}

IntroSatLib::intefaces::I2C::~I2C()
{
}

