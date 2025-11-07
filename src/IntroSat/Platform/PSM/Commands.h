/*
 * Commands.h
 *
 *  Created on: Oct 21, 2025
 *      Author: alesh
 */

#ifndef INTROSAT_PLATFORM_PSM_COMMANDS_H_
#define INTROSAT_PLATFORM_PSM_COMMANDS_H_

#include <stdint.h>

namespace platform::psm::commands {

constexpr uint8_t CMD_PING = 0x10; 	// should be sent in Init();

	constexpr uint8_t ACK = 0x5A;	// sPM answers with ACK if the command was parsed and processed correctly;]
	constexpr uint8_t NACK = 0xA5;


constexpr uint8_t CMD_SET_MODE_HEATER = 0x20; // Set heater operating mode
	constexpr uint8_t MODE_HEATER_AUTO = 0x0;
	constexpr uint8_t MODE_HEATER_MANUAL = 0x1;
	constexpr uint8_t MODE_HEATER_OFF = 0x2;

constexpr uint8_t CMD_SET_HEATER = 0x30; // Set
	constexpr uint8_t HEATER_ON = 0xF;
	constexpr uint8_t HEATER_OFF = 0x0;

//constexpr bool IS_HEATER

constexpr uint8_t CMD_SET_MODE_POWER = 0x40;
	constexpr uint8_t MODE_POWER_OFF = 0x0;
	constexpr uint8_t MODE_POWER_MANUAL = 0x1; // uses jumpers to determine channel state
	constexpr uint8_t MODE_POWER_NORMAL = 0x2; // uses uart commands to determine channel state
	constexpr uint8_t MODE_POWER_ALL_ON = 0xF;

constexpr uint8_t CMD_ENABLE_POWER_CHANNEL = 0x50;
constexpr uint8_t CMD_DISABLE_POWER_CHANNEL = 0x60;
constexpr uint8_t CMD_GET_POWER_CHANNEL_INFO = 0x70;
constexpr uint8_t CMD_GET_EXTENDED_POWER_CHANNEL_INFO = 0x80;

constexpr uint8_t POWER_CHANNEL_CRC_MASK = 0x01; // If debil - send NACK
enum PowerChannel: uint8_t {
	POWER_CHANNEL_12V = 0x1,
	POWER_CHANNEL_3V3 = 0x2,
	POWER_CHANNEL_3V3_IS = 0x3,
	POWER_CHANNEL_5V = 0x4,
	POWER_CHANNEL_5V_IS = 0x5
};

constexpr bool IS_POWER_CHANNEL(uint8_t __arg__) {
	return (__arg__ & POWER_CHANNEL_CRC_MASK) > 0;
}
constexpr uint8_t POWER_CHANNEL_INDEX_FROM_ARG(uint8_t  __arg__) {
	return (__arg__ >> 1);
}

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
	EnableChannel(uint8_t channel) {
		if (IS_POWER_CHANNEL(channel)) req |= (channel | POWER_CHANNEL_CRC_MASK);
	}
};

struct DisableChannel: PSMCommand<uint8_t> {
	uint8_t req = CMD_DISABLE_POWER_CHANNEL;
	DisableChannel(uint8_t channel) {
		if (IS_POWER_CHANNEL(channel)) req |= (channel | POWER_CHANNEL_CRC_MASK);
	}
};

struct GetChannelInfo: PSMCommand<ChannelInfo> {
	uint8_t req = CMD_GET_POWER_CHANNEL_INFO;
	GetChannelInfo(uint8_t channel) {
		if (IS_POWER_CHANNEL(channel)) req |= (channel | POWER_CHANNEL_CRC_MASK);
	}
};

struct GetExtendedChannelInfo: PSMCommand<ExtendedChannelInfo> {
	uint8_t req = CMD_GET_EXTENDED_POWER_CHANNEL_INFO;
	GetExtendedChannelInfo(uint8_t channel) {
		if (IS_POWER_CHANNEL(channel)) req |= (channel | POWER_CHANNEL_CRC_MASK);
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

#endif /* INTROSAT_PLATFORM_PSM_COMMANDS_H_ */
