#define ISL_INTERNAL

#include "Adapter/I2C.h"

#ifdef ISL_I2C_ENABLED

#include "GyroscopeV1.h"
#include "IntroSatLib_def.h"
#include "Device/I2CDevice.h"

namespace IntroSatLib {

GyroscopeV1::GyroscopeV1(interfaces::I2C i2c, uint8_t address): I2CDevice(i2c, address)
{
}
	
GyroscopeV1::GyroscopeV1(const GyroscopeV1 &other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
GyroscopeV1::GyroscopeV1(GyroscopeV1 &&other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
GyroscopeV1& GyroscopeV1::operator=(const GyroscopeV1 &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
GyroscopeV1& GyroscopeV1::operator=(GyroscopeV1 &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

ISL_StatusTypeDef GyroscopeV1::Init(Scale sensitivity, FilterBandwidth filter)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	RETURN_STATUS_IF_NOT_OK(IsReady(), status);
	RETURN_STATUS_IF_NOT_OK(SetRegisterI2C(0x37, 0x02), status);
	RETURN_STATUS_IF_NOT_OK(SetScale(sensitivity), status);
	return SetFilter(filter);
}
ISL_StatusTypeDef GyroscopeV1::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0005);
}
ISL_StatusTypeDef GyroscopeV1::Init()
{
	return Init(Scale::DPS0250);
}

ISL_StatusTypeDef GyroscopeV1::SetScale(Scale sensitivity)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg));
	reg &= 0xFF ^ (Scale::DPS2000 << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg);
}

ISL_StatusTypeDef GyroscopeV1::SetFilter(FilterBandwidth filter)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg));
	reg &= 0xFF ^ 3;
	reg |= (filter >> 3);
	RETURN_STATUS_IF_NOT_OK(SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg), status)

	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CONFIG, &reg));
	reg &= 0xFF ^ 7;
	reg |= (filter & 7);
	return SetRegisterI2C(RegisterMap::CONFIG, reg);
}

int16_t GyroscopeV1::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t GyroscopeV1::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t GyroscopeV1::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float GyroscopeV1::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawdps;
}
float GyroscopeV1::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawdps;
}
float GyroscopeV1::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawdps;
}


GyroscopeV1::~GyroscopeV1() { }

}

#endif /* ISL_I2C_ENABLED */