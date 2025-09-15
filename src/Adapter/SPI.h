#ifndef ADAPTER_SPI_H_
#define ADAPTER_SPI_H_

// Include general Library definitions
#include "IntroSatLib_def.h"

#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include <SPI.h>
	namespace IntroSatLib::interfaces {using SPI_HANDLE_TYPE = SPIClass;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	// Resolve platform-dependent SPI
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

		#ifdef HAL_SPI_MODULE_ENABLED
			// define STM32-specific handle type for SPI
			namespace IntroSatLib::interfaces {using SPI_HANDLE_TYPE = SPI_HandleTypeDef;}
		#elif !defined(INTROSATLIB_INTERNAL)
			#error "SPI not enabled as part of HAL"
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

// #ifdef SPI_HANDLE_TYPE
#define SPI_ENABLED

#include "Logger.h"

#define ASSERT_SPI_HAVE() \
if(!_hspi) { \
logText("No spi handle"); \
return ISL_StatusTypeDef::ISL_ERROR; \
}

#define LOG_SPI(mode) \
logText("SPI "); \
logText(mode);

#if LOGDATA
#define LOG_SPI_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
logHEX(Data[i]); \
if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_SPI_BUFFER(Sep, Data, Nbytes)
#endif

#if __has_include(<array>)
#define STL_AVAILABLE
#include <array>
#endif

#include "Adapter/GPIO.h"

namespace IntroSatLib {
namespace interfaces {

class SPI final {
	SPI_HANDLE_TYPE *_hspi = 0;
	::IntroSatLib::interfaces::GPIO* _cs;

	bool _useInternalCs = false;
	bool _csActiveHigh = false;

	ISL_StatusTypeDef _transfer(const uint8_t* out, uint8_t* in, uint8_t len);
public:
 	SPI(SPI_HANDLE_TYPE *hspi): _hspi(hspi) { };
	SPI(SPI_HANDLE_TYPE &hspi): _hspi(&hspi) { };

// STL not available in Arduino IDE by default
#ifdef STL_AVAILABLE
 	template<size_t N>
 	ISL_StatusTypeDef transfer(const std::array<uint8_t, N> out, std::array<uint8_t, N> in)
 	{
 		return transfer(out.data(), in.data(), N);
 	}
 	template<size_t N>
 	ISL_StatusTypeDef transfer(const std::array<uint8_t, N> out, uint8_t* in)
 	{
 		return transfer(out.data(), in, N);
 	}

 	template<size_t N>
 	ISL_StatusTypeDef transfer(const uint8_t* out, std::array<uint8_t, N> in)
 	{
 		return transfer(out, in.data(), N);
 	}
#endif

 	ISL_StatusTypeDef transfer(const uint8_t* out, uint8_t* in, uint8_t len) {
		ASSERT_SPI_HAVE();
	
		if (_useInternalCs) {
			logText("Asserting CS");
			_cs->write(_csActiveHigh);
		}
		
		LOG_SPI("read/write");
		LOG_SPI_BUFFER(", ", out, len);
		logText(" ");
		logNumber((uint8_t)len);
		logText(" bytes > ");

		ISL_StatusTypeDef result = _transfer(out, in, len);

		LOG_SPI_BUFFER(", ", in, len);
		logText("\n");

		if (_useInternalCs) {
			logText("De-asserting CS");
			_cs->write(!_csActiveHigh);
		}

	}

	ISL_StatusTypeDef setCs(GPIO_HANDLE_TYPE* port, uint16_t pin, bool activeHigh = false) {
		_cs = new interfaces::GPIO(port, pin);
    	_csActiveHigh = activeHigh;
	}
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

// #endif /* SPI_HANDLE_TYPE */

#endif /* ADAPTER_SPI_H_ */
