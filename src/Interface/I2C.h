#ifndef INTERFACE_I2C_H_
#define INTERFACE_I2C_H_

// Resolve platform-dependent I2C

/////////////// AVR  ////////////////////
// AVR-series in Arduino IDE
#if defined(AVR) && defined(ARDUINO)
	#error "AVR not yet supported"
	#include "Arduino.h"
	#include "Wire.h"


#elif defined(USE_HAL_DRIVER)
/////////  STM32 and stm32duino //////////

	#ifdef ARDUINO
		// Arduino IDE with stm32duino
		#include "Arduino.h"
		#include "Wire.h"
	#endif

	#include "STM32/stm32XXyy_hal.h"
	// Supported STM32 platforms
	#ifndef HAL_MODULE_ENABLED
		#error "HAL not enabled"
	#endif

	#ifdef HAL_I2C_MODULE_ENABLED
		// include STM32-specific implementation of I2C
		#include "STM32/STM32_I2C.h"
	#elif !defined(INTROSATLIB_INTERNAL)
		#error "I2C not enabled as part of HAL"
	#endif
#else
///////////// UNKNOWN ///////////////////
	#error "Unsupported system"
	#error "Currently supported systems are: stm32, stm32duino"
#endif

#endif /* INTERFACE_I2C_H_ */
