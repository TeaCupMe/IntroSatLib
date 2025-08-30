/*
 * Accelerometer.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "../Device.h"

namespace IntroSatLib {

class BaseAccelerometer: virtual public Device {

public:
	virtual int16_t RawAX() = 0;
	virtual int16_t RawAY() = 0;
	virtual int16_t RawAZ() = 0;

	virtual float AX() = 0;
	virtual float AY() = 0;
	virtual float AZ() = 0;
};

} /* namespace IntroStratLib */


#endif /* ACCELEROMETER_H_ */
