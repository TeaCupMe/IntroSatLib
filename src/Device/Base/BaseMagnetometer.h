/*
 * Magnetometer.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "Device/Device.h"


namespace IntroSatLib {

class BaseMagnetometer : public Device {

public:

	virtual int16_t RawMX() = 0;
	virtual int16_t RawMY() = 0;
	virtual int16_t RawMZ() = 0;

	virtual float MX() = 0;
	virtual float MY() = 0;
	virtual float MZ() = 0;

};

} /* namespace IntroSatLib */


#endif /* MAGNETOMETER_H_ */
