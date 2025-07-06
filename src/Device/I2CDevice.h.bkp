#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <stdint.h>

#ifdef ARDUINO
	#include "Arduino.h"
	#include "Wire.h"
#else
	#if __has_include ("stm32f4xx_hal.h")
		#include "stm32f4xx_hal.h"
		#include "stm32f4xx_hal_i2c.h"
	#endif

	#if __has_include ("stm32f1xx_hal.h")
		#include "stm32f1xx_hal.h"
		#include "stm32f1xx_hal_i2c.h"
	#endif
#endif


namespace IntroSatLib {

enum class I2CSpeed
{
	Standard = 0,
	Fast = 1
};

class I2CDevice {
private:
	I2CSpeed _speed = I2CSpeed::Standard;
	uint8_t _address = 0;
	I2C_HandleTypeDef *_hi2c = 0;
	HAL_StatusTypeDef innerIsReady();
public:
#ifndef ARDUINO
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C 
	 */
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C 
	 * @param speed скорость I2C
	 */
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed);
#else
	/**
	 * @note Только в Arduino IDE
	 * @brief Конструктор объекта гироскопа
	 * 
	 * @param hi2c объект @b TwoWire или @b Wire 
	 * @param address адрес гироскопа на шине I2C
	 */
	I2CDevice(TwoWire &hi2c, uint8_t address);

	/**
	 * @note Только в Arduino IDE
	 * @brief Конструктор объекта устройства на шине I2C
	 * 
	 * @param hi2c объект @b TwoWire или @b Wire 
	 * @param address адрес устройства на шине I2C 
	 * @param speed скорость I2C
	 */
	I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed);
#endif
	I2CDevice(const I2CDevice& other);
	I2CDevice(I2CDevice&& other);
	I2CDevice& operator=(const I2CDevice& other);
	I2CDevice& operator=(I2CDevice&& other);

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
	~I2CDevice();
};

} /* namespace IntroSatLib */

#endif /* I2CDEVICE_H_ */
