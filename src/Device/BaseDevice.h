#ifndef BASEDEVICE_H_
#define BASEDEVICE_H_

#include "I2CDevice.h"

namespace IntroSatLib {

class BaseDevice {

protected:
	I2CDevice &_i2c;

	/**
	 * @brief Чтение значения из регистра устройства
	 * 
	 * @param reg Адрес регистра
	 * @return Значение решистра
	 */
	uint8_t GetRegister(uint8_t reg);

	/**
	 * @brief Запись значения в регистр устройства
	 * 
	 * @param reg Адрес регистра
	 * @param value Значение
	 */
	void SetRegister(uint8_t reg, uint8_t value);

	/**
	 * @brief Установка одного бита регистра в значение 1
	 * 
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 */
	void SetBitRegister(uint8_t reg, uint8_t bit);

	/**
	 * @brief Установка одного бита регистра в значение 0
	 * 
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 */
	void ResetBitRegister(uint8_t reg, uint8_t bit);

	/**
	 * @brief Установка одного бита регистра в значение по условию @ref value
	 * 
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 * @param value Условие установки. Если true (&ne; 0) - выставляет в 1, иначе - в 0.
	 */
	void BitRegister(uint8_t reg, uint8_t bit, uint8_t value);

public:
#ifndef ARDUINO
	/**
	 * @brief Создание объекта @b @ref BaseDevice
	 * @note Только в STM32CubeIDE
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	BaseDevice(I2C_HandleTypeDef *hi2c, uint8_t address);
#else
	/**
	 * @brief Создание объекта @ref BaseDevice
	 * @note Только в Arduino IDE
	 * 
	 * @param hi2c объект @b TwoWire или @b Wire
	 * @param address адрес устройства на шине I2C
	 */
	BaseDevice(TwoWire &hi2c, uint8_t address);

	/**
	 * @brief Создание объекта @ref BaseDevice на @b I2C1
	 * @note Только в Arduino IDE
	 * 
	 * @param address адрес устройства на шине I2C
	 */
	BaseDevice(uint8_t address);
#endif
	/**
	 * @brief Создание объекта @ref BaseDevice как копии другого объекта @ref BaseDevice 
	 * @note Только в Arduino IDE
	 * 
	 * @param address адрес устройства на шине I2C
	 */
	BaseDevice(const BaseDevice& other);
	BaseDevice(BaseDevice&& other);
	BaseDevice& operator=(const BaseDevice& other);
	BaseDevice& operator=(BaseDevice&& other);

	/**
	 * @brief Инициализация устройства на шине I2C
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns код ошибки HAL_StatusTypeDef, если при инициализации возникла ошибка 
	 */
	virtual uint8_t Init();
	virtual ~BaseDevice();
};

} /* namespace IntroSatLib */

#endif /* BASEDEVICE_H_ */
