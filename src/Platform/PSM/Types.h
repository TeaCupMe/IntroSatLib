#ifndef MODULES_PLATFORM_PSM_TYPES_H_
#define MODULES_PLATFORM_PSM_TYPES_H_
#include <stdint.h>
#include <stddef.h>
namespace platform::psm::types {

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

struct __attribute__((__packed__)) HeaterInfo {
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
	uint16_t rawVoltage;
	uint16_t rawCurrent;

	uint32_t lastDisabled;
	uint32_t lastOvercurrent;
	uint32_t lastUndervoltage;
	uint32_t lastEnabled;
};

struct __attribute__((__packed__)) ChannelConfig {
	ChannelState state = ChannelState::Off;
	bool enableOvercurrentProtection = false;
	bool enableUndervoltageProtection = false;
	float overCurrentThreshold = 0.f;
	float underVoltageThreshold = 0.f;
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

static constexpr size_t powerChannelCount{static_cast<size_t>(PowerChannel::Qty)};

static constexpr float kRailVoltage3v3 = 3.3f;
static constexpr float kRailVoltage5v = 5.0f;

// SET_OVERCURRENT_PROTECTION low nibble: 0 = off, 1..15 = amps (max 2.5 A).
static constexpr float kOvercurrentPresetAmps[16] = {
	0.f,
	0.10f, 0.20f, 0.30f, 0.40f, 0.50f,
	0.60f, 0.75f, 0.90f, 1.00f, 1.25f,
	1.50f, 1.75f, 2.00f, 2.25f, 2.50f
};

// SET_UNDERVOLTAGE_PROTECTION low nibble: 0 = off, 1..15 = percent of 3.3/5 V nominal.
// UV is accepted only on Main/Payload 3v3 and 5v channels.
static constexpr uint8_t kUndervoltagePresetPercent[16] = {
	0,
	95, 90, 85, 80, 75,
	70, 65, 60, 55, 50,
	45, 40, 35, 30, 25
};

static constexpr uint8_t kProtectionPresetCount = 16;

static_assert(sizeof(kOvercurrentPresetAmps) / sizeof(kOvercurrentPresetAmps[0]) == kProtectionPresetCount);
static_assert(sizeof(kUndervoltagePresetPercent) / sizeof(kUndervoltagePresetPercent[0]) == kProtectionPresetCount);
static_assert(kOvercurrentPresetAmps[kProtectionPresetCount - 1] == 2.50f);

static_assert(sizeof(ChannelInfo) == 13, "ChannelInfo wire size");
static_assert(sizeof(ExtendedChannelInfo) == 33, "ExtendedChannelInfo wire size");
static_assert(sizeof(HeaterInfo) == 9, "HeaterInfo wire size");
static_assert(sizeof(ChannelConfig) == 11, "ChannelConfig packed size");

} /* namespace platform::psm::types */

#endif /* MODULES_PLATFORM_PSM_TYPES_H_ */
