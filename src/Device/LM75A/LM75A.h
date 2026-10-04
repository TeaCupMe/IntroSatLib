/*
 * LM75A.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef LM75A_H_
#define LM75A_H_
#include "Device/Base/BaseTemperatureSensor.h"
#include "Device/I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
//#include <functional>


namespace IntroSatLib {



class LM75A: public BaseTemperatureSensor, protected I2CDevice {
private:
	enum RegisterMap: uint8_t {
			TEMPERATURE = 0x00,
			CONFIGURATION = 0x01,
			T_HYST = 0x02,
			T_OS = 0x03
		};

	static const uint8_t BASE_ADDRESS = 0x48;

public:
	/**
	 * \~english @brief Construct a new LM75A object
	 * \~russian @brief Создание нового объекта LM75A
	 * 
	 * \~english @param i2c - The I2C interface to use. Wire in Arduino IDE, I2C_HandleTypeDef in STM32CubeIDE
	 * \~russian @param i2c - Интерфейс I2C для использования. Wire в Arduino IDE, I2C_HandleTypeDef в STM32CubeIDE
	 * \~english @param address - The address of the device on the I2C bus. Default is 0x48
	 * \~russian @param address - Адрес устройства на шине I2C. По умолчанию 0x48
	 */
	LM75A(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * \~english @brief Initialize the LM75A sensor
	 * \~russian @brief Инициализация датчика LM75A
	 * 
	 * \~english @return ISL_StatusTypeDef - The status of the initialization. ISL_OK if successful, ISL_ERROR otherwise 
	 * \~russian @return ISL_StatusTypeDef - Статус инициализации. ISL_OK при успешной инициализации, ISL_ERROR в противном случае
	 */
	ISL_StatusTypeDef Init();

	/**
	 * \~english @brief Get the raw temperature value
	 * \~russian @brief Получить значение температуры в исходном формате
	 * 
	 * \~english @return int16_t - The raw temperature value
	 * \~russian @return int16_t - Значение температуры в исходном формате
	 */
	int16_t GetRawTemperature(); // TODO: make private

	/**
	 * \~english @brief Get the temperature in Celsius times 8
	 * \~russian @brief Получить температуру в градусах Цельсия умноженную на 8
	 * 
	 * \~english @return int16_t - The temperature in Celsius times 8
	 * \~russian @return int16_t - Температура в градусах Цельсия умноженная на 8
	 * 
	 * \~english @note This method is optimized for the LM75A sensor and returns temperature in uint16_t instead of float, whithout loss of precision. 
	 * \~russian @note Этот метод оптимизирован для датчика LM75A и возвращает температуру в uint16_t вместо float, без потери точности.
	 */
	int16_t GetTemperatureTimes8();

	/**
	 * \~english @brief Get the temperature in Celsius
	 * \~russian @brief Получить температуру в градусах Цельсия
	 * 
	 * \~english @return float - The temperature in Celsius
	 * \~russian @return float - Температура в градусах Цельсия
	 */
	float GetTemperature() override;
	uint8_t GetConfig();

	void PowerDown(bool shutdown);

};


}




#endif /* LM75A_H_ */
