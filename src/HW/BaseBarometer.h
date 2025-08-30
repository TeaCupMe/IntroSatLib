	/*
 * Barometer.h
 *
 *  Created on: Mar 7, 2025
 *      Author: unflesh
 */

#ifndef BAROMETER_H_
#define BAROMETER_H_

#include "Device/Device.h"

namespace IntroStratLib {

class BaseBarometer: public Device {

public:
	virtual uint32_t GetRawPressure() = 0;
	virtual uint32_t GetRawTemperature() = 0;
	virtual float GetPressure() = 0;
	virtual float GetTemperature() = 0;
};

} /* namespace IntroStratLib */


#endif /* BAROMETER_H_ */
