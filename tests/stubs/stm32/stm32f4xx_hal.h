/**
 * @file stm32f4xx_hal.h
 * @brief Minimal STM32 HAL type stubs for host unit tests.
 *
 * Production Adapter headers include this via stm32xxyy_hal.h when a chip
 * macro such as STM32F401xC is defined. These stubs exist only so headers
 * compile on a host toolchain; driver tests never call into HAL.
 */
#ifndef INTROSATLIB_TEST_STUB_STM32F4XX_HAL_H_
#define INTROSATLIB_TEST_STUB_STM32F4XX_HAL_H_

#include <stdint.h>

#ifndef HAL_MODULE_ENABLED
#define HAL_MODULE_ENABLED
#endif
#ifndef HAL_I2C_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#endif
#ifndef HAL_SPI_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#endif
#ifndef HAL_GPIO_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#endif
#ifndef HAL_UART_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#endif

typedef struct {
	uint32_t unused;
} I2C_HandleTypeDef;

typedef struct {
	uint32_t unused;
} SPI_HandleTypeDef;

typedef struct {
	uint32_t unused;
} UART_HandleTypeDef;

typedef struct {
	uint32_t unused;
} GPIO_TypeDef;

typedef enum {
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET = 1
} GPIO_PinState;

#endif /* INTROSATLIB_TEST_STUB_STM32F4XX_HAL_H_ */
