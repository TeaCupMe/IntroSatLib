#define ISL_INTERNAL

#include "Adapter/I2C.h"

#ifdef ISL_I2C_ENABLED

#include "AccelerometerV1.h"
#include "Device/I2CDevice.h"

namespace IntroSatLib {

AccelerometerV1::AccelerometerV1(interfaces::I2C i2c, uint8_t address): I2CDevice(i2c, address)
{
}

AccelerometerV1::AccelerometerV1(const AccelerometerV1& other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
AccelerometerV1::AccelerometerV1(AccelerometerV1&& other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
AccelerometerV1& AccelerometerV1::operator=(const AccelerometerV1& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
AccelerometerV1& AccelerometerV1::operator=(AccelerometerV1&& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

ISL_StatusTypeDef AccelerometerV1::Init(Scale sensitivity, FilterBandwidth filter)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x37, 0x02));
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(sensitivity));
	return SetFilter(filter);
	//TODO  возвращать _i2c.isReady();
//	return 0;
}
ISL_StatusTypeDef AccelerometerV1::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0021);
}
ISL_StatusTypeDef AccelerometerV1::Init()
{
	return Init(Scale::twoG);
}


ISL_StatusTypeDef AccelerometerV1::SetScale(Scale sensitivity)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK(ReadRegisterI2C(RegisterMap::ACCEL_CONFIG, &reg), status);
	reg &= 0xFF ^ (Scale::sixteenG << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	return SetRegisterI2C(RegisterMap::ACCEL_CONFIG, reg);
}

ISL_StatusTypeDef AccelerometerV1::SetFilter(FilterBandwidth filter)
{
	return SetRegisterI2C(RegisterMap::ACCEL_CONFIG_2, filter);
}


int16_t AccelerometerV1::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t AccelerometerV1::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t AccelerometerV1::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float AccelerometerV1::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawg;
}
float AccelerometerV1::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawg;
}
float AccelerometerV1::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawg;
}

AccelerometerV1::~AccelerometerV1() { }

}

#endif /* ISL_I2C_ENABLED */