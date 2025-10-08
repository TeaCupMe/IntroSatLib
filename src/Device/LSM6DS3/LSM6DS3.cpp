/*
 * LSM6DS3.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: Goldfor
 */

#include "LSM6DS3.h"
#include "Adapter/System.h"
#include "stdint.h"

namespace IntroSatLib {

LSM6DS3::LSM6DS3(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

ISL_StatusTypeDef LSM6DS3::WhoAmI() {
	uint8_t iAm;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::WHO_AM_I_REG, &iAm));
	return (ISL_StatusTypeDef) (iAm != 0x69);
}

ISL_StatusTypeDef LSM6DS3::InitGyro(ScaleGyro sensitivityGyro, DataRateGyro dataRateGyro, uint8_t force)
{
	system::Delay(15); // wait for power up
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())
	RETURN_STATUS_IF_NOT_OK_SILENT(WhoAmI())
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScaleGyro(sensitivityGyro))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetDataRateGyro(dataRateGyro))
	return IsReady();
}

ISL_StatusTypeDef LSM6DS3::InitGyro(ScaleGyro sensitivityGyro, uint8_t force)
{
	return InitGyro(sensitivityGyro, DataRateGyro::DR_G_416_Hz, force);
}
ISL_StatusTypeDef LSM6DS3::InitGyro(uint8_t force)
{
	return InitGyro(ScaleGyro::DPS0500, force);
}

ISL_StatusTypeDef LSM6DS3::InitAccel(ScaleAccel sensitivityAccel, DataRateAccel dataRateAccel, uint8_t force)
{
	system::Delay(15); // wait for power up
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())
	RETURN_STATUS_IF_NOT_OK_SILENT(WhoAmI())
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScaleAccel(sensitivityAccel))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetDataRateAccel(dataRateAccel))
	return IsReady();
}

ISL_StatusTypeDef LSM6DS3::InitAccel(ScaleAccel sensitivityAccel, uint8_t force)
{
	return InitAccel(sensitivityAccel, DataRateAccel::DR_A_104_Hz, force);
}
ISL_StatusTypeDef LSM6DS3::InitAccel(uint8_t force)
{
	return InitAccel(ScaleAccel::fourG, force);
}

void LSM6DS3::SetMinCutX(float x)
{
	_cutX = x;
}
void LSM6DS3::SetMinCutY(float y)
{
	_cutY = y;
}
void LSM6DS3::SetMinCutZ(float z)
{
	_cutZ = z;
}

float LSM6DS3::cutMin(float value, float cut)
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

ISL_StatusTypeDef LSM6DS3::SetScaleGyro(ScaleGyro sensitivityGyro)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1));
	reg &= 0xFF ^ (ScaleGyro::DPS2000 << 2);
	reg |= (sensitivityGyro << 2);
	_sensitivityGyro = sensitivityGyro;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1);
}

ISL_StatusTypeDef LSM6DS3::SetDataRateGyro(DataRateGyro dataRateGyro)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1))
	reg &= 0x0F;
	reg |= (dataRateGyro << 4);
	_dataRateGyro = dataRateGyro;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1);
}

ISL_StatusTypeDef LSM6DS3::SetScaleAccel(ScaleAccel sensitivityAccel) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1));
	reg &= 0xFF ^ (ScaleAccel::eightG << 2);
	reg |= (sensitivityAccel << 2);
	switch (sensitivityAccel)
	{
		case ScaleAccel::twoG:
			_sensitivityAccel = 0;
			break;
	    case ScaleAccel::fourG:
	    	_sensitivityAccel = 1;
	    	break;
	    case ScaleAccel::eightG:
	    	_sensitivityAccel = 2;
	    	break;
	    case ScaleAccel::sixteenG:
	    	_sensitivityAccel = 3;
	    	break;
	  }
	return SetRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1);
}


ISL_StatusTypeDef LSM6DS3::SetDataRateAccel(DataRateAccel dataRateAccel) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1));
	reg &= 0xFF ^ (0x0F<<4);
	reg |= (dataRateAccel<<4);
	_dataRateAccel = dataRateAccel;
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
}

int16_t LSM6DS3::RawGX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::GX()
{
	float e = RawGX() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutX);
}

float LSM6DS3::GY()
{
	float e = RawGY() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutY);
}
float LSM6DS3::GZ()
{
	float e = RawGZ() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutZ);
}

int16_t LSM6DS3::RawAX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTX_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTY_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTZ_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::AX ()
{
	float e = RawAX() * (1 << _sensitivityAccel) * _rawg;
	return e;
}
float LSM6DS3::AY()
{
	float e = RawAY() * (1 << _sensitivityAccel) * _rawg;
	return e;
}
float LSM6DS3::AZ()
{
	float e = RawAZ() * (1 << _sensitivityAccel) * _rawg;
	return e;
}

float LSM6DS3::Temp() {
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUT_TEMP_L, buf, 2);
	return float((int16_t)(buf[1] << 8 | buf[0]) / 16.0 + 25);
}

ISL_StatusTypeDef LSM6DS3::DeinitAccel() {
	return SetRegisterI2C(RegisterMap::CTRL1_XL, 0x00);
}

ISL_StatusTypeDef LSM6DS3::DeinitGyro() {
	return SetRegisterI2C(RegisterMap::CTRL2_G, 0x00);
}

ISL_StatusTypeDef LSM6DS3::Deinit() {
	RETURN_STATUS_IF_NOT_OK_SILENT(DeinitAccel());
	return DeinitGyro();
}

LSM6DS3::~LSM6DS3() { }

} /* namespace IntroSatLib */
