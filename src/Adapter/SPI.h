#ifndef ADAPTER_SPI_H_
#define ADAPTER_SPI_H_

// Resolve platform-dependent SPI
#ifdef ARDUINO
#include "Arduino.h"
#endif

#if defined(AVR)
/************** AVR  **************/
	//  This is not yet supported, but it is here for future reference.
	//  AVR-series in Arduino IDE
	#error "AVR not yet supported"
	#include "SPI.h"

#elif defined(USE_HAL_DRIVER)
/*****  STM32 and stm32duino ******/

	#ifdef ARDUINO
		// Arduino IDE with stm32duino
		#include "SPI.h"
	#endif

	// Include hal for supported STM32 platforms
	#include "STM32/stm32XXyy_hal.h"

	#ifndef HAL_MODULE_ENABLED
		#error "HAL not enabled"
	#endif

	#ifdef HAL_SPI_MODULE_ENABLED
		// include STM32-specific implementation of I2C
		#include "STM32/STM32_SPI.h"
//		#define SPI_HANDLE_TYPE SPI_HandleTypeDef
	#elif !defined(INTROSATLIB_INTERNAL)
		#error "SPI not enabled as part of HAL"
	#endif

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

#endif /* ADAPTER_SPI_H_ */
