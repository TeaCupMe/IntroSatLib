#ifndef ADAPTER_I2C_H_
#define ADAPTER_I2C_H_

#include "IntroSatLib_def.h"
// Resolve platform-dependent I2C

#if defined(AVR) && defined(ARDUINO)
/************** AVR  **************/
	//  This is not yet supported, but it is here for future reference.
	//  AVR-series in Arduino IDE
	#error "AVR not yet supported"
	#include "Arduino.h"
	#include "Wire.h"

#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
/*****  STM32 and stm32duino ******/

	#ifdef ARDUINO
		// Arduino IDE with stm32duino
		#include "Arduino.h"
		#include "Wire.h"
	#endif
	
	// Include hal for supported STM32 platforms
	#include "STM32/stm32XXyy_hal.h"

	#ifndef HAL_MODULE_ENABLED
		#error "HAL not enabled"
	#endif

	#ifdef HAL_I2C_MODULE_ENABLED
		// include STM32-specific implementation of I2C
//		#include "STM32/STM32_I2C.h"
		#define I2C_HANDLE_TYPE I2C_HandleTypeDef
	#elif !defined(INTROSATLIB_INTERNAL)
		#error "I2C not enabled as part of HAL"
	#endif

/************** AMUR *************/
// #elif defined(AMUR)
	// #error "AMUR not yet supported"
/************ UNKNOWN ************/
#else
#ifndef INTROSAT_INTERNAL
	#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
	#error "Currently supported systems are: stm32, stm32duino. AVR planned for future support."
#endif
#endif

#ifdef I2C_HANDLE_TYPE
#define I2C_ENABLED

#include <array>

namespace IntroSatLib {
namespace interfaces {

class I2C final {
	enum class I2CSpeed
	{
		Standard = 0,
		Fast = 1
	};
private:
	I2C_HANDLE_TYPE *_hi2c = 0;
	I2CSpeed _speed = I2CSpeed::Standard;
	ISL_StatusTypeDef innerIsReady(uint8_t address);
public:
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2C(I2C_HANDLE_TYPE *hi2c);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2C(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed);

#ifdef ARDUINO
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2C(TwoWire &hi2c);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2C(TwoWire &hi2c, I2CSpeed speed);
#endif

	I2C(const I2C& other);
	I2C(I2C&& other);
	I2C& operator=(const I2C& other);
	I2C& operator=(I2C&& other);

	/**
	 * @brief Проверка доступности устройства на шине к работе
	 *
	 * @param waitIsReady
	 * @returns 0, если устройство доступно
	 * @returns 1, если с устройством произошла ошибка
	 */
	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт с устройства
	 *
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в которую будет записан результат. В случае
	 * @param[in] Nbytes Количество байт  для чтения
	 * @return Статус выполнения чтения
	 */
	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт из регистра Register устройства
	 *
	 * @param[in] Register Адрес регистра для чтения
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в который будет записан результат. В случае
	 * @param[in] Nbytes Количество байт для чтения
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт на устройство
	 *
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт в регистр Register устройства
	 *
	 * @param Register Адрес регистра для записи
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);
	~I2C();
};

} /* namespace intefaces */
} /* namespace IntroSatLib */
#endif /* I2C_HANDLE_TYPE */
#endif /* ADAPTER_I2C_H_ */
