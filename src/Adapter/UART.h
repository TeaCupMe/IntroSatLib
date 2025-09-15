</**
 * replace <INTERFACE> with interface name in upper-case
 * replace <interface> with interface name in lower-case
 * replace <INTERFACE_ARDUINO_CLASS> with corresponding Arduino API class
 */>
#ifndef ADAPTER_UART_H_
#define ADAPTER_UART_H_

// Include general Library definitions
#include "IntroSatLib_def.h"

#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include <UART.h>
	namespace IntroSatLib::interfaces {using UART_HANDLE_TYPE = HardwareSerial;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	// Resolve platform-dependent UART
	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series in Arduino IDE
		#error "AVR not yet supported"

	#elif defined(USE_HAL_DRIVER)
	/*****  STM32 and stm32duino ******/

		// Include hal for supported STM32 platforms
		#include "./STM32/stm32xxyy_hal.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL configuration incorrect"
		#endif

		#ifdef HAL_UART_MODULE_ENABLED
			// define STM32-specific handle type for UART
			namespace IntroSatLib::interfaces {using UART_HANDLE_TYPE = UART_HandleTypeDef;}
		#elif !defined(INTROSATLIB_INTERNAL)
			#error "UART not enabled as part of HAL"
		#endif

	/************** AMUR *************/
	// #elif defined(AMUR)
		// #error "AMUR not yet supported"
	/************ UNKNOWN ************/
	#else
	// #ifndef INTROSATLIB_INTERNAL
		#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
		#error "Currently supported systems are: stm32, stm32duino. AVR planned for future support."
	// #endif
	#endif
#endif

// #ifdef UART_HANDLE_TYPE
#define UART_ENABLED

namespace IntroSatLib {
namespace interfaces {

class UART final {
	UART_HANDLE_TYPE *_huart = 0;
public:
 	UART(UART_HANDLE_TYPE *huart): _huart(huart) { };
	UART(UART_HANDLE_TYPE &huart): _huart(&huart) { };
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

// #endif /* UART_HANDLE_TYPE */

#endif /* ADAPTER_UART_H_ */
