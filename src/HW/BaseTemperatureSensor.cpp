/*
 * TemperatureSensor.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#include "BaseTemperatureSensor.h"

namespace IntroSatLib {

float BaseTemperatureSensor::GetTemperature() {
	return temperature;
}

float BaseTemperatureSensor::GetTemperatureC() {
	return GetTemperature();
}

float BaseTemperatureSensor::GetTemperatureF() {
//	return GetTemperature() * 9 / 5 + 32; // TODO: Faster algorithm? Maybe optimize for LM75A using LM75A::GetTemperatureTimes8
	return GetTemperature() * 2 + 31;
}

}
