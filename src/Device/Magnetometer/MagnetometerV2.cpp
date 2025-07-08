#include "MagnetometerV2.h"

namespace IntroSatLib {

//#ifndef ARDUINO
MagnetometerV2::MagnetometerV2(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}
//#else
//MagnetometerV2::MagnetometerV2(TwoWire &hi2c, uint8_t address): I2CDevice(hi2c, address)
//{
//}
//MagnetometerV2::MagnetometerV2(uint8_t address): I2CDevice(address)
//{
//}
//#endif

MagnetometerV2::MagnetometerV2(const MagnetometerV2& other): I2CDevice(other)
{
}
MagnetometerV2::MagnetometerV2(MagnetometerV2&& other): I2CDevice(other)
{
}
MagnetometerV2& MagnetometerV2::operator=(const MagnetometerV2& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
	}
	return *this;
}
MagnetometerV2& MagnetometerV2::operator=(MagnetometerV2&& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
	}
	return *this;
}


ISL_StatusTypeDef MagnetometerV2::Init(Scale sensitivity)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	if ((status = SetRegisterI2C(RegisterMap::CTRL_REG1, 0x7C)) != ISL_StatusTypeDef::ISL_OK) { return status; }
	HAL_Delay(1);
	if ((status = SetScale(sensitivity)) != ISL_StatusTypeDef::ISL_OK) { return status; }
	HAL_Delay(1);
	if ((status = SetRegisterI2C(RegisterMap::CTRL_REG3, 0x00)) != ISL_StatusTypeDef::ISL_OK) { return status; }
	HAL_Delay(1);
	if ((status = SetRegisterI2C(RegisterMap::CTRL_REG4, 0x0C)) != ISL_StatusTypeDef::ISL_OK) { return status; }
	HAL_Delay(1);
	status = SetRegisterI2C(RegisterMap::CTRL_REG5, 0x40);
	return status;
}

ISL_StatusTypeDef MagnetometerV2::Init()
{
	return Init(Scale::G16);
}	

ISL_StatusTypeDef MagnetometerV2::SetScale(Scale sensitivity)
{
	uint8_t bitSensitivity = 2 * (sensitivity - 1);
	uint8_t reg = bitSensitivity << 4;
	_sensitivity = sensitivity;
	return SetRegisterI2C(RegisterMap::CTRL_REG2, reg);
}

ISL_StatusTypeDef MagnetometerV2::Read()
{
//	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
//	uint8_t status_reg;
//
//	if ((status = ReadRegisterI2C(RegisterMap::STATUS_REG, &status_reg)) != 0) { return status; }
//
//	if (status_reg & 0x08)
//	{
//		uint8_t buf[6];
//		_i2c.read(RegisterMap::OUT_X_L, buf, 6);
//		_x = buf[1] << 8 | buf[0];
//		_y = buf[3] << 8 | buf[2];
//		_z = buf[5] << 8 | buf[4];
//	}
//	return ISL_StatusTypeDef::ISL_ERROR;

	// Return ISL_OK not to break anything
	return ISL_StatusTypeDef::ISL_OK;
}

int16_t MagnetometerV2::RawX()
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t status_reg;

	if ((status = ReadRegisterI2C(RegisterMap::STATUS_REG, &status_reg)) != 0) { return 0; }

	if (status_reg & (0x1 << 0))
	{
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_X_L, buf, 2);
		return (buf[1] << 8 | buf[0]);
	}
	return 0;
}
int16_t MagnetometerV2::RawY()
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t status_reg;

	if ((status = ReadRegisterI2C(RegisterMap::STATUS_REG, &status_reg)) != 0) { return 0; }

	if (status_reg & (0x1 << 1))
	{
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_Y_L, buf, 2);
		return (buf[1] << 8 | buf[0]);
	}
	return 0;
}

int16_t MagnetometerV2::RawZ()
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t status_reg;

	if ((status = ReadRegisterI2C(RegisterMap::STATUS_REG, &status_reg)) != 0) { return 0; }

	if (status_reg & (0x1 << 2))
	{
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_Z_L, buf, 2);
		return (buf[1] << 8 | buf[0]);
	}
	return 0;
}

float MagnetometerV2::X()
{
	float e = int16_t(_x) * _sensitivity;
	return e / _rawg;
}
float MagnetometerV2::Y()
{
	float e = int16_t(_y) * _sensitivity;
	return e / _rawg;	
}
float MagnetometerV2::Z()
{
	float e = int16_t(_z) * _sensitivity;
	return e / _rawg;	
}

#ifndef ARDUINO
Quaternion<float> MagnetometerV2::GetQuaternion()
{
	std::array<float, 3> buf;
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = std::atan2(RawY(), RawX());
	return from_euler(buf);
}
#endif

MagnetometerV2::~MagnetometerV2() { }

} /* namespace IntroSatLib */
