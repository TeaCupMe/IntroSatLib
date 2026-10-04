/*
 * LIS3MDL.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef LIS3MDL_H_
#define LIS3MDL_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseMagnetometer.h"

namespace IntroSatLib {

/**
 * \~english @brief LIS3MDL magnetometer sensor driver
 * \~russian @brief Драйвер датчика магнитного поля LIS3MDL 
 */
class LIS3MDL : public BaseMagnetometer, protected I2CDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x1C;
	static constexpr float _rawg = 27386.0f / 4.0f; // Gauss

	enum RegisterMap
	{
		CTRL_REG1 = 0x20,
		CTRL_REG2,
		CTRL_REG3,
		CTRL_REG4,
		CTRL_REG5,
		STATUS_REG = 0x27,
		OUT_X_L,
		OUT_X_H,
		OUT_Y_L,
		OUT_Y_H,
		OUT_Z_L,
		OUT_Z_H,
	};

	uint16_t _mx = 0;
	uint16_t _my = 0;
	uint16_t _mz = 0;

	uint8_t _scale = 0;

public:

	/**
	 * \~english @brief Scale of the magnetometer
	 * \~russian @brief Диапазон измерений магнитометра
	 */
	enum Scale
	{
		G4 = 1, /** \~english @brief &plusmn;4 gauss scale 	\~russian @brief Диапазон &plusmn;4 гаусса */
		G8, 	/** \~english @brief &plusmn;8 gauss scale 	\~russian @brief Диапазон &plusmn;8 гаусса */
		G12, 	/** \~english @brief &plusmn;12 gauss scale \~russian @brief Диапазон &plusmn;12 гаусса */
		G16 	/** \~english @brief &plusmn;16 gauss scale \~russian @brief Диапазон &plusmn;16 гаусса */
	};

	/**
	 * \~english @brief Construct a new LIS3MDL object
	 * \~russian @brief Создание новый объект LIS3MDL
	 * 
	 * \~english @param i2c - The I2C interface to use. Wire in Arduino IDE, I2C_HandleTypeDef in STM32CubeIDE
	 * \~russian @param i2c - объект интерфейса I2C - Wire в Arduino IDE, I2C_HandleTypeDef в STM32CubeIDE
	 * \~english @param address - device address on the I2C bus. Default is 0x1C
	 * \~russian @param address - адрес устройства на шине I2C. По умолчанию 0x1C
	 */
	LIS3MDL(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * \~english @brief Initialize the LIS3MDL sensor
	 * \~russian @brief Инициализация датчика LIS3MDL
	 * 
	 * \~english @param scale - The scale to initialize with
	 * \~russian @param scale - Диапазон для инициализации
	 */
	ISL_StatusTypeDef Init(Scale scale);

	/**
	 * \~english @brief Initialize the LIS3MDL sensor with the default scale of &plusmn;16 gauss
	 * \~russian @brief Инициализация датчика LIS3MDL с использованием шкалы по умолчанию в &plusmn;16 гаусс
	 */
	ISL_StatusTypeDef Init() override {
		return Init(Scale::G16);
	}

	/**
	 * \~english @brief Set the scale of the LIS3MDL sensor
	 * \~russian @brief Установка диапазона измерений датчика LIS3MDL
	 * 
	 * \~english @param scale - The scale to set
	 * \~russian @param scale - Диапазон для установки
	 */
	ISL_StatusTypeDef SetScale(Scale scale);

	/**
	 * \~english @brief Get the raw magnetometer X-axis value
	 * \~russian @brief Получить значение магнитометра по оси X в исходном формате
	 */
	int16_t RawMX() override;
	/**
	 * \~english @brief Get the raw magnetometer Y-axis value
	 * \~russian @brief Получить значение магнитометра по оси Y в исходном формате
	 */
	int16_t RawMY() override;
	/**
	 * \~english @brief Get the raw magnetometer Z-axis value
	 * \~russian @brief Получить значение магнитометра по оси Z в исходном формате
	 */
	int16_t RawMZ() override;

	/**
	 * \~english @brief Get the magnetometer X-axis value in gauss
	 * \~russian @brief Получить значение магнитометра по оси X в гауссах
	 */
	float MX() override;
	/**
	 * \~english @brief Get the magnetometer Y-axis value in gauss
	 * \~russian @brief Получить значение магнитометра по оси Y в гауссах
	 */
	float MY() override;
	/**
	 * \~english @brief Get the magnetometer Z-axis value in gauss
	 * \~russian @brief Получить значение магнитометра по оси Z в гауссах
	 */
	float MZ() override;

	/**
	 * \~english @brief Disable the LIS3MDL sensor
	 * \~russian @brief Отключение датчика LIS3MDL
	 */
	ISL_StatusTypeDef Disable() override;
	/**
	 * \~english @brief Enable the LIS3MDL sensor
	 * \~russian @brief Включение датчика LIS3MDL
	 */
	ISL_StatusTypeDef Enable() override;

	~LIS3MDL();
};

} /* namespace IntroStratLib */

#endif /* LIS3MDL_H_ */
