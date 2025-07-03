#ifndef MAGNETOMETER_V2_H_
#define MAGNETOMETER_V2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"
#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#endif
#include <cmath>
#include <array>

namespace IntroSatLib
{

	class MagnetometerV2 : public BaseDevice
	{
	private:
		static const uint8_t BASE_ADDRESS = 0x1E;
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

		uint16_t _x = 0;
		uint16_t _y = 0;
		uint16_t _z = 0;

		uint8_t _sensitivity = 0;

	public:

	/**
	 * @brief Диапазон измерения
	 * 
	 */
		enum Scale 
		{
			G4 = 1, /**< Диапазон &plusmn;4 Гаусс */
			G8,		/**< Диапазон &plusmn;8 Гаусс */
			G12,	/**< Диапазон &plusmn;12 Гаусс */
			G16		/**< Диапазон &plusmn;16 Гаусс */
		};	

#ifndef ARDUINO
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Создание объекта манитометра. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес манитометра на шине I2C
	 */
		MagnetometerV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
		/**
		 * @note Только в Arduino IDE
		 * @brief Создание объекта манитометра
		 * 
		 * @param hi2c объект @b TwoWire или @b Wire 
		 * @param address адрес манитометра на шине I2C
		 */
		MagnetometerV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
		
		/**
		 * @note Только в Arduino IDE
		 * @brief Создание объекта манитометра на @b I2C1 
		 * 
		 * @param address адрес манитометра на шине I2C
		 */
		MagnetometerV2(uint8_t address = BASE_ADDRESS);
#endif
		/**
		 * @brief Создание объекта манитометра как копии другого объекта манитометра
		 * 
		 * @param other исходный объект для копирования
		 */
		MagnetometerV2(const MagnetometerV2 &other);
		MagnetometerV2 &operator=(const MagnetometerV2 &other);
		MagnetometerV2(MagnetometerV2 &&other);
		MagnetometerV2 &operator=(MagnetometerV2 &&other);

		/**
		 * @brief Инициализация манитометра с параметрами по умолчанию: @ref Scale::G16
		 * 
		 * @returns 0, если инициализация завершена успешно
		 * @returns 1, если при инициализации возникла ошибка 
		 */
		uint8_t Init() override;

		/**
		 * @brief Инициализация манитометра с заданным диапазоном измерения
		 * 
		 * @param sensitivity Значение чуствительности @ref Scale
		 * @returns 0, если инициализация завершена успешно
		 * @returns 1, если при инициализации возникла ошибка
		 */
		uint8_t Init(Scale sensitivity);

		/**
		 * @brief Установка диапазона измерения
		 * 
		 * @param sensitivity Значение чуствительности @ref Scale
		 */
		void SetScale(Scale sensitivity);

		/**
		 * @brief Чтение значения с датчика во внутренний буфер
		 * 
		 */
		void Read();

		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси X
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси X
		 */
		int16_t RawX();
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси Y
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси Y
		 */
		int16_t RawY();
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси Z
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси Z
		 */
		int16_t RawZ();

		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси X
		 * 
		 * @return Значение напряжённости магнитного поля по оси X
		 */
		float X();
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси Y
		 * 
		 * @return Значение напряжённости магнитного поля по оси Y
		 */
		float Y();
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси Z
		 * 
		 * @return Значение напряжённости магнитного поля по оси Z
		 */
		float Z();
#ifndef ARDUINO	
		// TODO @Goldfor @TeaCupMe Что тут получаем?
		Quaternion<float> GetQuaternion();
#endif

		~MagnetometerV2() override;
	};

} /* namespace IntroSatLib */

#endif /* MAGNETOMETER_V2_H_ */
