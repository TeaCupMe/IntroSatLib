#ifndef INTERFACE_SPI_H_
#define INTERFACE_SPI_H_



// TODO Generalize preprocessor directives
#ifdef ARDUINO
	#include "Arduino.h"
	#include "../Interface/SPI.h"
	#define	SPI_ENABLED
#else
	#if __has_include ("stm32f4xx_hal_spi.h")
		#include "stm32f4xx_hal.h"
		#include "stm32f4xx_hal_spi.h"
		#define	SPI_ENABLED
	#elif __has_include ("stm32f1xx_hal_spi.h")
		#include "stm32f1xx_hal.h"
		#include "stm32f1xx_hal_spi.h"
		#define	SPI_ENABLED
	#elif __has_include ("stm32h7xx_hal_spi.h")
		#include "stm32h7xx_hal.h"
		#include "stm32h7xx_hal_spi.h"
		#define	SPI_ENABLED
	#endif
#endif


#ifdef SPI_ENABLED // If SPI is enabled or at least accessible (in case of Arduino IDE)


#include <array>


namespace IntroSatLib {
namespace intefaces {

class SPI final {
	SPI_HandleTypeDef *_hspi = 0;
public:
	SPI(SPI_HandleTypeDef *hspi): _hspi(hspi) { };

	template<size_t N>
	HAL_StatusTypeDef transfer(const std::array<uint8_t, N> out, std::array<uint8_t, N> in)
	{
		return transfer(out.data(), in.data(), N);
	}
	template<size_t N>
	HAL_StatusTypeDef transfer(const std::array<uint8_t, N> out, uint8_t* in)
	{
		return transfer(out.data(), in, N);
	}

	template<size_t N>
	HAL_StatusTypeDef transfer(const uint8_t* out, std::array<uint8_t, N> in)
	{
		return transfer(out, in.data(), N);
	}

	HAL_StatusTypeDef transfer(const uint8_t* out, uint8_t* in, uint8_t len)
	{
		LOG_SPI("read/write");
		LOG_SPI_BUFFER(", ", out, len);
		logText(" ");
		logNumber((uint8_t)len);
		logText(" bytes > ");
		HAL_StatusTypeDef result = logStatus(
			HAL_SPI_TransmitReceive(_hspi, (uint8_t*) out, in, len, 1000)
		);

		LOG_SPI_BUFFER(", ", in, len);
		logText("\n");
		return result;
	}
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

#endif /* SPI_ENABLED */
#endif /* INTERFACE_SPI_H_ */


