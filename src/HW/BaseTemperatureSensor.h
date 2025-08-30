/*
 * TemperatureSensor.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef TEMPERATURESENSOR_H_
#define TEMPERATURESENSOR_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseTemperatureSensor: public Device {

private:

	float temperature;
public:
	virtual float GetTemperature();
	virtual float GetTemperatureC();
	virtual float GetTemperatureF();
	/*virtual*/ void enable();
};
}




#endif /* TEMPERATURESENSOR_H_ */
