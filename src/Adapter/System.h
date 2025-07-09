#ifndef ADAPTER_SYSTEM_H_
#define ADAPTER_SYSTEM_H_

#include "IntroSatLib_def.h"
// Resolve platform-dependent I2C
#ifdef ARDUINO
#include "Arduino.h"
#endif


#if defined(AVR)
/************** AVR  **************/
	//  This is not yet supported, but it is here for future reference.
	//  AVR-series in Arduino IDE
	#error "AVR not yet supported"

#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
/*****  STM32 and stm32duino ******/

	#ifdef ARDUINO
		// Arduino IDE with stm32duino
		#include "Arduino.h"
	#endif

	// Include hal for supported STM32 platforms
	#include "STM32/stm32XXyy_hal.h"

	#ifndef HAL_MODULE_ENABLED
		#error "HAL not enabled"
	#endif

	// include STM32-specific implementation of basic functions
	#include "STM32/STM32_System.h"


/************** AMUR *************/
// #elif defined(AMUR)
	// #error "AMUR not yet supported"
/************ UNKNOWN ************/
#else
#ifndef INTROSATLIB_INTERNAL
	#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
	#error "Currently supported systems are: stm32, stm32duino. AVR planned for future support."
#endif
#endif

#include <stdint.h>

namespace IntroSatLib {
	namespace system {
		uint32_t GetTick();
	};
};

#endif /* ADAPTER_SYSTEM_H_ */
