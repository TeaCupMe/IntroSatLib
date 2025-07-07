#include "AccelerometerV2.h"

namespace IntroSatLib {

AccelerometerV2::AccelerometerV2(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

ISL_StatusTypeDef AccelerometerV2::Init()
{
	if (!(CheckRegisterI2C(RegisterMap::WHO_AM_I_REG, 0x6C) || CheckRegisterI2C(RegisterMap::WHO_AM_I_REG, 0x69))) {
		return ISL_StatusTypeDef::ISL_ERROR;
	}
//	SetRegister(RegisterMap::CTRL1_XL, BASE_ACCEL_CONF);
	// TODO Лучше сделать явную установку параметров, так понятнее, что происходит, чем использование магического BASE_ACCEL_CONF
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(Scale::fourG));
	RETURN_STATUS_IF_NOT_OK_SILENT(SetDataRate(DataRate::DR6_66KH));
	return SetFilter(FilterBandwidth::F400H);
}

ISL_StatusTypeDef AccelerometerV2::Init(Scale sens)
{ 
	RETURN_STATUS_IF_NOT_OK_SILENT(Init());
	return SetScale(sens);
}

ISL_StatusTypeDef AccelerometerV2::Init(Scale sens, FilterBandwidth filter)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(Init());
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(sens));
	return SetFilter(filter);
}

ISL_StatusTypeDef AccelerometerV2::Init(Scale sens, FilterBandwidth filter, DataRate datarate)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(Init());
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(sens))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetFilter(filter));
	return SetDataRate(datarate);

}

ISL_StatusTypeDef AccelerometerV2::SetScale(Scale sens) {
	uint8_t reg = GetRegisterI2C(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (Scale::eightG << 2);
	reg |= (sens << 2);
	switch (sens)
	{
		case 0:
			_sens = 0;
			break;
		case 1:
			_sens = 3;
			break;
		case 2:
			_sens = 1;
			break;
		case 3:
			_sens = 2;
			break;
	}
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
//	return sens;
}

ISL_StatusTypeDef AccelerometerV2::SetFilter(FilterBandwidth filter) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg));
	reg &= 0xFF ^ (FilterBandwidth::F050H);
	reg |= (filter);
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
}

ISL_StatusTypeDef AccelerometerV2::SetDataRate(DataRate datarate) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg));
	reg &= 0xFF ^ (0x0F<<4);
	reg |= (datarate<<4);
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
}

ISL_StatusTypeDef AccelerometerV2::end() //отключение акселерометра
{
	return SetRegisterI2C(RegisterMap::CTRL1_XL, 0x00);
}

int16_t AccelerometerV2::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTX_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t AccelerometerV2::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTY_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t AccelerometerV2::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTZ_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}

float AccelerometerV2::X()
{
	float e = RawX() * (1 << _sens) / _rawg;
	return e;
}
float AccelerometerV2::Y()
{
	float e = RawY() * (1 << _sens) / _rawg;
	return e;
}
float AccelerometerV2::Z()
{
	float e = RawZ() * (1 << _sens) / _rawg;
	return e;
}

float AccelerometerV2::Temp() {
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUT_TEMP_L, buf, 2);
	return (int16_t)(buf[1] << 8 | buf[0]) / 16.0 + 25;
}

AccelerometerV2::~AccelerometerV2() { }

}
