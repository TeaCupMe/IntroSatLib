#ifndef ADAPTER_SYSTEM_H_
#define ADAPTER_SYSTEM_H_

// Include general Library definitions
#include "IntroSatLib_def.h"


#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
#else
/*********************************/
/************ Native *************/
/*********************************/

	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series in Arduino IDE
		#error "AVR not yet supported"

	#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
	/*****  STM32 and stm32duino ******/

		// Include hal for supported STM32 platforms
		#include "./STM32/stm32xxyy_hal.h"
		#include "dwt.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL not enabled"
		#endif


	/************** AMUR *************/
	// #elif defined(AMUR)
		// #error "AMUR not yet supported"

	#else
	/************ UNKNOWN ************/

		#ifndef ISL_INTERNAL
			#error Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros.  \
			 		Currently supported systems are: stm32 with HAL, stm32duino. AVR planned for future support.
		#endif
	#endif
#endif /* ARDUINO */

#include <stdint.h>
namespace IntroSatLib {
	namespace system {
		uint32_t GetTick();
		const auto GetMillis = GetTick; // Alias for GetTick, as millis() is more common in Arduino and HAL_GetTick() is more common in STM32 HAL
		uint32_t GetMicros();
		void Delay(uint32_t milliseconds);
		void DelayMicroseconds(uint32_t microseconds);
	};
};

#endif /* ADAPTER_SYSTEM_H_ */
