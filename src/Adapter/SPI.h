#ifndef ADAPTER_SPI_H_
#define ADAPTER_SPI_H_

// Resolve platform-dependent SPI

/////////////// AVR  ////////////////////
// AVR-series in Arduino IDE
#if defined(AVR) && defined(ARDUINO)
	#error "AVR not yet supported"
	#include "Arduino.h"
	#include "SPI.h"


#elif defined(USE_HAL_DRIVER)
/////////  STM32 and stm32duino //////////

	#ifdef ARDUINO
		// Arduino IDE with stm32duino
		#include "Arduino.h"
		#include "SPI.h"
	#endif

	#include "STM32/stm32XXyy_hal.h"
	// Supported STM32 platforms
	#ifndef HAL_MODULE_ENABLED
		#error "HAL not enabled"
	#endif

	#ifdef HAL_SPI_MODULE_ENABLED
		// include STM32-specific implementation of I2C
		#include "STM32/STM32_SPI.h"
	#elif !defined(INTROSATLIB_INTERNAL)
		#error "SPI not enabled as part of HAL"
	#endif
#else
///////////// UNKNOWN ///////////////////
	#error "Unsupported system"
	#error "Currently supported systems are: stm32, stm32duino"
#endif

#endif /* ADAPTER_SPI_H_ */
