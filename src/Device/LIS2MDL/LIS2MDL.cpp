/*
 * LIS2MDL.h
 *
 *  Created on: Nov 24, 2025
 *      Author: samsa
 */

#include "LIS2MDL.h"
#include "Adapter/System.h"

namespace IntroSatLib {


LIS2MDL::LIS2MDL(const interfaces::I2C &i2c): I2CDevice(new interfaces::I2C(i2c), BASE_ADDRESS)
{
}

ISL_StatusTypeDef LIS2MDL::Init()
{
	system::Delay(15); // wait for powerup (can be 10)
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady());
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CFG_REG_A, 0b10001100));
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CFG_REG_B, 0b00000000));
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CFG_REG_C, 0b00010000));
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::INT_CRTL_REG, 0b00000000));
	return IsReady();
}

int16_t LIS2MDL::RawMX()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG) & 0x01) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUTX_L_REG, buf, 2);
		_mx = buf[1] << 8 | buf[0];
	}
	return _mx;
}
int16_t LIS2MDL::RawMY()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG)&0x02) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUTY_L_REG, buf, 2);
		_my = buf[1] << 8 | buf[0];
	}
	return _my;
}
int16_t LIS2MDL::RawMZ()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG)&0x04) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUTZ_L_REG, buf, 2);
		_mz = buf[1] << 8 | buf[0];
	}
	return _mz;
}

float LIS2MDL::MX()
{
	RawMX();
	return _mx * _sens / 1000;
}
float LIS2MDL::MY()
{
	RawMY();
	return _my * _sens / 1000;
}
float LIS2MDL::MZ()
{
	RawMZ();
	return _mz * _sens / 1000;
}


LIS2MDL::~LIS2MDL() { }

} /* namespace IntroSatLib */



