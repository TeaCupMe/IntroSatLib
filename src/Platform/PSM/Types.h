#ifndef MODULES_PLATFORM_PSM_TYPES_H_
#define MODULES_PLATFORM_PSM_TYPES_H_
#include <stdint.h>
namespace platform::psm::types {

// Heater
enum class HeaterMode: uint8_t {
	Auto,
	Manual,
	Off,
};

enum class HeaterState: uint8_t {
	On,
	OnAuto,
	Off,
	OffAuto
};

struct HeaterInfo {
	HeaterMode heaterMode;
	HeaterState heaterState;
	uint8_t heaterPower;
	float heaterCurrent;
	uint16_t temperature;
};


enum class PowerMode: uint8_t {
	Manual,
	Normal,
	AllOn,
	AllOff
};

enum class ChannelState: uint8_t {
	Off,
	On,
	OffOvercurrent,
	OffUndervoltage
};

struct __attribute__((__packed__)) ChannelInfo {
	ChannelState state;
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
	//	float powerUsage; // continuous integration??
};

struct __attribute__((__packed__)) ChannelConfig {
	ChannelState state = ChannelState::Off;
	bool enableOvercurrentProtection = false;
	bool enableUndervoltageProtection = false;
	float overCurrentThreshold = INT64_MAX;
	float underVoltageThreshold = 0;
};

enum class PowerChannel: uint8_t {
	Battery,
	PayloadBattery,
	Main3v3,
	Payload3v3,
	Main5v,
	Payload5v,
	Qty
};

} /* namespace platform::psm::types */

#endif /* MODULES_PLATFORM_PSM_TYPES_H_ */
