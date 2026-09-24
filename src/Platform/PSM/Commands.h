#ifndef MODULES_PLATFORM_PSM_COMMANDS_H_
#define MODULES_PLATFORM_PSM_COMMANDS_H_

#include <stdint.h>

namespace platform::psm::commands
{

enum class Cmd : uint8_t
{
	Ping 			  = 0x10,
	SetHeaterMode 	  = 0x20,
	SetHeater 		  = 0x30,
	SetPowerMode 	  = 0x40,
	EnableChannel 	  = 0x50,
	DisableChannel 	  = 0x60,
	GetChannelInfo 	  = 0x70,
	GetExtChannelInfo = 0x80,
	GetTemp 		  = 0x90,
	GetHeaterInfo 	  = 0xA0,
	SafeCfg 		  = 0xB0,
	SelectChannel 	  = 0xC0,
	SetOC 			  = 0xD0,
	SetUV 			  = 0xE0
};

enum class Ack : uint8_t
{
	Ack  = 0x5A,	// sPM answers with ACK if the command was parsed and processed correctly;]
	Nack = 0xA5
};

}

#endif /* MODULES_PLATFORM_PSM_COMMANDS_H_ */
