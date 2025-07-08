#include "GyroscopeV2.h"

namespace IntroSatLib {

GyroscopeV2::GyroscopeV2(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

GyroscopeV2::GyroscopeV2(const GyroscopeV2 &other): I2CDevice(other)
{
	_scale = other._scale;
	_dataRate = other._dataRate;
}
GyroscopeV2::GyroscopeV2(GyroscopeV2 &&other): I2CDevice(other)
{
	_scale = other._scale;
	_dataRate = other._dataRate;
}
GyroscopeV2& GyroscopeV2::operator=(const GyroscopeV2 &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_scale= other._scale;
		_dataRate = other._dataRate;
	}
	return *this;
}
GyroscopeV2& GyroscopeV2::operator=(GyroscopeV2 &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_scale= other._scale;
		_dataRate = other._dataRate;
	}
	return *this;
}
ISL_StatusTypeDef GyroscopeV2::Init(Scale scale, DataRate dataRate)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	if ((status = SetScale(scale)) != ISL_StatusTypeDef::ISL_OK) { return status; }

	if ((status = SetDataRate(dataRate)) != ISL_StatusTypeDef::ISL_OK) { return status; }
	_lastXTime = system::GetTick();
	_lastYTime = _lastXTime;
	_lastZTime = _lastXTime;
	return status;
}

ISL_StatusTypeDef GyroscopeV2::Init(Scale scale)
{
	return Init(scale, DataRate::F_104_Hz);
}
ISL_StatusTypeDef GyroscopeV2::Init()
{
	return Init(Scale::DPS0250);
}

void GyroscopeV2::SetMinCutX(float x)
{
	_cutX = x;
}
void GyroscopeV2::SetMinCutY(float y)
{
	_cutY = y;
}
void GyroscopeV2::SetMinCutZ(float z)
{
	_cutZ = z;
}

float GyroscopeV2::cutMin(float value, float cut)
{
	if (value > 0)
	{
		return (value > cut) ? value - cut : 0;
	}
	else
	{
		return ((-value) > cut) ? value + cut : 0;
	}
}

ISL_StatusTypeDef GyroscopeV2::SetScale(Scale scale)
{
	uint8_t reg = GetRegisterI2C(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (Scale::DPS2000 << 2);
	reg |= (scale << 2);
	_scale = scale;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg);
}

ISL_StatusTypeDef GyroscopeV2::SetDataRate(DataRate dataRate)
{
	uint8_t reg = GetRegisterI2C(RegisterMap::GYRO_CONFIG);
	reg &= 0x0F;
	reg |= (dataRate << 4);
	_dataRate = dataRate;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg);
}

int16_t GyroscopeV2::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t GyroscopeV2::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t GyroscopeV2::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}

float GyroscopeV2::X()
{
	float e = RawX() * (1 << _scale);
	return cutMin(e * _rawdps, _cutX);
}

float GyroscopeV2::Y()
{
	float e = RawY() * (1 << _scale);
	return cutMin(e * _rawdps, _cutY);
}
float GyroscopeV2::Z()
{
	float e = RawZ() * (1 << _scale);
	return cutMin(e * _rawdps, _cutZ);
}

float GyroscopeV2::integrationX()
{
	float speed = X();
	uint32_t time = system::GetTick();
	uint32_t deltaTime = time - _lastXTime;
	float value = (_lastX + speed) * (deltaTime >> 1) * 0.001;
	_lastX = speed;
	_lastXTime = time;
	return value;
}

float GyroscopeV2::integrationY()
{
	float speed = Y();
	uint32_t time = system::GetTick();
	uint32_t deltaTime = time - _lastYTime;
	float value = (_lastY + speed) * (deltaTime >> 1) * 0.001;
	_lastY = speed;
	_lastYTime = time;
	return value;
}
float GyroscopeV2::integrationZ()
{
	float speed = X();
	uint32_t time = system::GetTick();
	uint32_t deltaTime = time - _lastZTime;
	float value = (_lastZ + speed) * (deltaTime >> 1) * 0.001;
	_lastZ = speed;
	_lastZTime = time;
	return value;
}

#ifndef ARDUINO
Quaternion<float> GyroscopeV2::GetQuaternion()
{
	std::array<float, 3> buf;
	buf[0] = integrationX();
	buf[1] = integrationY();
	buf[2] = integrationZ();
	return from_euler(buf);
}
#endif


GyroscopeV2::~GyroscopeV2() { }

}
