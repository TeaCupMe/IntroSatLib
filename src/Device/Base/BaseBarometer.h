/*
 * Barometer.h
 *
 *  Created on: Mar 7, 2025
 *      Author: unflesh
 */

#ifndef BAROMETER_H_
#define BAROMETER_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseBarometer: public Device {
	float pressure;
public:
	virtual float GetPressure() = 0;
};

} /* namespace IntroStratLib */


#endif /* BAROMETER_H_ */
