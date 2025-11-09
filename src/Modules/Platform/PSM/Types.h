/*
 * Types.h
 *
 *  Created on: Nov 8, 2025
 *      Author: alesh
 */

#ifndef MODULES_PLATFORM_PSM_TYPES_H_
#define MODULES_PLATFORM_PSM_TYPES_H_
#include <stdint.h>
namespace platform::psm::types {

// Heater
enum HeaterMode: uint8_t {
	HEATER_MODE_AUTO = 0x00,
	HEATER_MODE_MANUAL = 0x01,
	HEATER_MODE_OFF = 0x02
};

enum HeaterState: uint8_t {
	HEATER_STATE_ON = 0x1,
	HEATER_STATE_ON_AUTO = 0x3,
	HEATER_STATE_OFF = 0x0,
	HEATER_STATE_OFF_AUTO = 0x2
};

struct HeaterInfo {
	HeaterMode heaterMode;
	HeaterState heaterState;
	uint8_t heaterPower;
	float heaterCurrent;
	uint16_t temperature;
};


enum PowerMode: uint8_t {
	POWER_MODE_MANUAL = 0x0,
	POWER_MODE_NORMAL = 0x3, //
	POWER_MODE_ALL_ON = 0x6,
	POWER_MODE_ALL_OFF = 0x9
};

enum PowerChannelState: uint8_t {
	DISABLED = 0x0,
	ENABLED = 0x1,
	DISABLED_OVERCURRENT = 0x2,
	DISABLED_UNDERVOLTAGE = 0x3
};

struct __attribute__((__packed__)) ChannelInfo {
	PowerChannelState state;
	float voltage;
	float current;
	float peakCurrent;
};

struct __attribute__((__packed__)) ExtendedChannelInfo: ChannelInfo {
	uint16_t rawVoltage; // 12-bit from ADC
	uint16_t rawCurrent; // 12-bit from ADC

	uint32_t lastDisabled = 0;
	uint32_t lastOvercurrent = 0;
	uint32_t lastUndervoltage = 0;
	uint32_t lastEnabled = 0;

	bool enableOvercurrentProtection = false;
	bool enableUndervoltageProtection = false;
	float overCurrentThreshold = INT64_MAX;
	float underVoltageThreshold = 0;
	//	float powerUsage; // continuous integration??
};

} /* namespace platform::psm::types */

#endif /* MODULES_PLATFORM_PSM_TYPES_H_ */
