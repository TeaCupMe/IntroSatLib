#ifndef MODULES_PLATFORM_PSM_COMMANDS_H_
#define MODULES_PLATFORM_PSM_COMMANDS_H_

#include <stdint.h>

namespace platform::psm::commands {

enum CMD {
	PING = 0x10,
	SET_HEATER_MODE = 0x20,
	SET_HEATER = 0x30,
	SET_POWER_MODE = 0x40,
	ENABLE_POWER_CHANNEL = 0x50,
	DISABLE_POWER_CHANNEL = 0x60,
	GET_POWER_CHANNEL_INFO = 0x70,
	GET_EXTENDED_POWER_CHANNEL_INFO = 0x80,
	GET_TEMP_INT16 = 0x90,
	GET_TEMP_FLOAT = 0xA0,
	GET_HEATER_INFO = 0xB0
};

constexpr uint8_t ACK = 0x5A;	// sPM answers with ACK if the command was parsed and processed correctly;]
constexpr uint8_t NACK = 0xA5;



}

#endif /* MODULES_PLATFORM_PSM_COMMANDS_H_ */
