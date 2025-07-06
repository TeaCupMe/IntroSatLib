#ifndef ADAPTER_CLOCK_H_
#define ADAPTER_CLOCK_H_

#include "IntroSatLib_def.h"
// Resolve platform-dependent I2C

#if defined(AVR) && defined(ARDUINO)
/************** AVR  **************/
	//  This is not yet supported, but it is here for future reference.
	//  AVR-series in Arduino IDE
	#error "AVR not yet supported"
	#include "Arduino.h"

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
	#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
	#error "Currently supported systems are: stm32, stm32duino. AVR planned for future support."
#endif

#endif /* ADAPTER_CLOCK_H_ */
