/*
 * Commands.h
 *
 *  Created on: Oct 21, 2025
 *      Author: alesh
 */

#ifndef MODULES_PLATFORM_PSM_COMMANDS_H_
#define MODULES_PLATFORM_PSM_COMMANDS_H_

#include <stdint.h>

namespace platform::psm::commands {
using namespace platform::psm::types;

constexpr uint8_t CMD_PING = 0x10; 	// should be sent in Init();

	constexpr uint8_t ACK = 0x5A;	// sPM answers with ACK if the command was parsed and processed correctly;]
	constexpr uint8_t NACK = 0xA5;


constexpr uint8_t CMD_SET_HEATER_MODE = 0x20; // Set heater operating mode
	constexpr uint8_t ARG_HEATER_MODE_AUTO 		= HeaterMode::HEATER_MODE_AUTO;
	constexpr uint8_t ARG_HEATER_MODE_MANUAL	= HeaterMode::HEATER_MODE_MANUAL;
	constexpr uint8_t ARG_HEATER_MODE_OFF 		= HeaterMode::HEATER_MODE_OFF;

constexpr bool IS_HEATER_MODE(uint8_t __arg__) {
	return 	(__arg__ == ARG_HEATER_MODE_AUTO) 	||
			(__arg__ == ARG_HEATER_MODE_MANUAL) ||
			(__arg__ == ARG_HEATER_MODE_OFF);
}

constexpr uint8_t CMD_SET_HEATER = 0x30; // Set
	constexpr uint8_t ARG_HEATER_ON = 0xF;
	constexpr uint8_t ARG_HEATER_OFF = 0x0;

//constexpr bool IS_HEATER

constexpr uint8_t CMD_SET_POWER_MODE = 0x40;
	constexpr uint8_t ARG_POWER_MODE_ALL_OFF = PowerMode::POWER_MODE_ALL_OFF;
	constexpr uint8_t ARG_POWER_MODE_MANUAL  = PowerMode::POWER_MODE_MANUAL; // uses jumpers to determine channel state
	constexpr uint8_t ARG_POWER_MODE_NORMAL  = PowerMode::POWER_MODE_NORMAL; // uses uart commands to determine channel state
	constexpr uint8_t ARG_POWER_MODE_ALL_ON  = PowerMode::POWER_MODE_ALL_ON;

constexpr bool IS_POWER_MODE(uint8_t __arg__) {
	return 	(__arg__ == ARG_POWER_MODE_ALL_OFF) ||
			(__arg__ == ARG_POWER_MODE_ALL_ON)  ||
			(__arg__ == ARG_POWER_MODE_NORMAL)  ||
			(__arg__ == ARG_POWER_MODE_MANUAL);
}

constexpr uint8_t CMD_ENABLE_POWER_CHANNEL = 0x50;
constexpr uint8_t CMD_DISABLE_POWER_CHANNEL = 0x60;
constexpr uint8_t CMD_GET_POWER_CHANNEL_INFO = 0x70;
constexpr uint8_t CMD_GET_EXTENDED_POWER_CHANNEL_INFO = 0x80;

//constexpr uint8_t POWER_CHANNEL_CRC_MASK = 0x01; // If debil - send NACK

	constexpr uint8_t ARG_POWER_CHANNEL_12V = PowerChannel::POWER_CHANNEL_12V;
	constexpr uint8_t ARG_POWER_CHANNEL_12V_IS = PowerChannel::POWER_CHANNEL_12V_IS;
	constexpr uint8_t ARG_POWER_CHANNEL_3V3 = PowerChannel::POWER_CHANNEL_3V3;
	constexpr uint8_t ARG_POWER_CHANNEL_3V3_IS = PowerChannel::POWER_CHANNEL_3V3_IS;
	constexpr uint8_t ARG_POWER_CHANNEL_5V = PowerChannel::POWER_CHANNEL_5V;
	constexpr uint8_t ARG_POWER_CHANNEL_5V_IS = PowerChannel::POWER_CHANNEL_5V_IS;

constexpr bool IS_POWER_CHANNEL(uint8_t __arg__) {
	return  (__arg__ == ARG_POWER_CHANNEL_12V) 	  ||
			(__arg__ == ARG_POWER_CHANNEL_12V_IS) ||
			(__arg__ == ARG_POWER_CHANNEL_3V3) 	  ||
			(__arg__ == ARG_POWER_CHANNEL_3V3_IS) ||
			(__arg__ == ARG_POWER_CHANNEL_5V) 	  ||
			(__arg__ == ARG_POWER_CHANNEL_5V_IS);
}
//constexpr bool IS_POWER_CHANNEL(uint8_t __arg__) {
//	return (__arg__ & POWER_CHANNEL_CRC_MASK) > 0;
//}
//constexpr uint8_t POWER_CHANNEL_INDEX_FROM_ARG(uint8_t  __arg__) {
//	return (__arg__ >> 1);
//}

constexpr uint8_t CMD_GET_TEMP_INT16 = 0x90;
constexpr uint8_t CMD_GET_TEMP_FLOAT = 0xA0;

constexpr uint8_t CMD_GET_HEATER_INFO = 0xB0;

template <typename reqType, typename respType>
struct Command {
	reqType req;
	respType res;
	bool ok;
};

template <typename respType>
using PSMCommand = Command<uint8_t, respType>;

struct GetTemperatureInt16: PSMCommand<uint16_t> {
	uint8_t req = CMD_GET_TEMP_INT16;
};

struct GetTemperatureFloat: PSMCommand<float> {
	uint8_t req = CMD_GET_TEMP_FLOAT;
};

struct EnableChannel: PSMCommand<uint8_t> {
	uint8_t req = CMD_ENABLE_POWER_CHANNEL;
	EnableChannel(PowerChannel channel) {
		req |= (channel);
	}
};

struct DisableChannel: PSMCommand<uint8_t> {
	uint8_t req = CMD_DISABLE_POWER_CHANNEL;
	DisableChannel(PowerChannel channel) {
		req |= (channel);
	}
};

struct GetChannelInfo: PSMCommand<ChannelInfo> {
	uint8_t req = CMD_GET_POWER_CHANNEL_INFO;
	GetChannelInfo(PowerChannel channel) {
		req |= (channel);
	}
};

struct GetExtendedChannelInfo: PSMCommand<ExtendedChannelInfo> {
	uint8_t req = CMD_GET_EXTENDED_POWER_CHANNEL_INFO;
	GetExtendedChannelInfo(PowerChannel channel) {
		req |= (channel);
	}
};

struct GetHeaterInfo: PSMCommand<HeaterInfo> {
	uint8_t req = CMD_GET_HEATER_INFO;
};

struct SetHeater: PSMCommand<uint8_t> {
	uint8_t req = CMD_GET_HEATER_INFO;
};

struct SetHeaterMode: PSMCommand<uint8_t> {
	uint8_t req = CMD_GET_HEATER_INFO;
};



}

#endif /* MODULES_PLATFORM_PSM_COMMANDS_H_ */
