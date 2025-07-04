#ifndef INTERFACES_I2C_H_
#define INTERFACES_I2C_H_


// TODO Generalize preprocessor directives
#ifdef ARDUINO
	#include "Arduino.h"
	#include "Wire.h"
	#define	I2C_ENABLED
#else
	#if __has_include ("stm32f4xx_hal_i2c.h")
		#include "stm32f4xx_hal.h"
		#include "stm32f4xx_hal_i2c.h"
		#define	I2C_ENABLED
	#elif __has_include ("stm32f1xx_hal_i2c.h")
		#include "stm32f1xx_hal.h"
		#include "stm32f1xx_hal_i2c.h"
		#define	I2C_ENABLED
	#elif __has_include ("stm32h7xx_hal_i2c.h")
		#include "stm32h7xx_hal.h"
		#include "stm32h7xx_hal_i2c.h"
		#define	I2C_ENABLED
	#endif
#endif


#ifdef I2C_ENABLED // If I2C is enabled or at least accessible (in case of Arduino IDE)

#include <array>



namespace IntroSatLib {
namespace intefaces {

class I2C final {
	enum class I2CSpeed
	{
		Standard = 0,
		Fast = 1
	};
private:
	I2CSpeed _speed = I2CSpeed::Standard;
	uint8_t _address = 0;
	I2C_HandleTypeDef *_hi2c = 0;
	HAL_StatusTypeDef innerIsReady();
public:
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2C(I2C_HandleTypeDef *hi2c, uint8_t address);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2C(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed);
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
	HAL_StatusTypeDef isReady(uint8_t waitIsReady = 0);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт с устройства
	 *
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в которую будет записан результат. В случае
	 * @param[in] Nbytes Количество байт  для чтения
	 * @return Статус выполнения чтения
	 */
	HAL_StatusTypeDef read(uint8_t* Data, uint8_t Nbytes);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт из регистра Register устройства
	 *
	 * @param[in] Register Адрес регистра для чтения
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в который будет записан результат. В случае
	 * @param[in] Nbytes Количество байт для чтения
	 * @return HAL_StatusTypeDef
	 */
	HAL_StatusTypeDef read(uint8_t Register, uint8_t* Data, uint8_t Nbytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт на устройство
	 *
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	HAL_StatusTypeDef write(uint8_t* Data, uint8_t Nbytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт в регистр Register устройства
	 *
	 * @param Register Адрес регистра для записи
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	HAL_StatusTypeDef write(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
	~I2C();
};

} /* namespace intefaces */
} /* namespace IntroSatLib */
#endif /* I2C_ENABLED */
#endif /* INTERFACES_I2C_H_ */
