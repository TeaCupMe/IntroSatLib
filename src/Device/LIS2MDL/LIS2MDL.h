/*
 * LIS2MDL.h
 *
 *  Created on: Nov 24, 2025
 *      Author: samsa
 */

#ifndef LIS2MDL_H_
#define LIS2MDL_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseMagnetometer.h"

namespace IntroSatLib {

class LIS2MDL : public BaseMagnetometer, protected I2CDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x1E;
	const float _sens = 1.5f; // mGauss/LSB

	enum RegisterMap
	{
		OFFSET_X_REG_L = 0x45,
        OFFSET_X_REG_H,
        OFFSET_Y_REG_L,
        OFFSET_Y_REG_H,
        OFFSET_Z_REG_L,
        OFFSET_Z_REG_H,
        WHO_AM_I = 0x4F,
        CFG_REG_A = 0x60,
        CFG_REG_B,
        CFG_REG_C,
        INT_CRTL_REG,
        INT_SOURCE_REG,
        INT_THS_L_REG,
        INT_THS_H_REG,
        STATUS_REG,
        OUTX_L_REG,
        OUTX_H_REG,
        OUTY_L_REG,
        OUTY_H_REG,
        OUTZ_L_REG,
        OUTZ_H_REG,
        TEMP_OUT_L_REG,
        TEMP_OUT_H_REG,
	};

	int16_t _mx = 0;
	int16_t _my = 0;
	int16_t _mz = 0;

public:

	LIS2MDL(const interfaces::I2C &i2c);

	ISL_StatusTypeDef Init();

	int16_t RawMX() override;
	int16_t RawMY() override;
	int16_t RawMZ() override;

	float MX() override;
	float MY() override;
	float MZ() override;

	~LIS2MDL();
};

} /* namespace IntroStratLib */

#endif /* LIS2MDL_H_ */
