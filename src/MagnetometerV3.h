#ifndef MAGNETOMETERV3_H_
#define MAGNETOMETERV3_H_

//#include "Device/I2CDevice.h"
//#include "../BaseDevice.h"
#include "Device/LIS2MDL/LIS2MDL.h"
#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#include <cmath>
#include <array>
#endif

namespace IntroSatLib
{

class MagnetometerV3 : private LIS2MDL
{
private:
	static const uint8_t BASE_ADDRESS = 0x1E;
	static const uint8_t LIS2MDL_WHO_AM_I = 0x40;

	uint16_t _x = 0;
	uint16_t _y = 0;
	uint16_t _z = 0;

public:

	/**
	 * @brief Создание объекта манитометра. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес манитометра на шине I2C
	 */
	MagnetometerV3(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS): LIS2MDL(i2c, address) {};

	/**
	 * @brief Создание объекта манитометра как копии другого объекта манитометра
	 * 
	 * @param other исходный объект для копирования
	 */
	MagnetometerV3(const MagnetometerV3 &other);
	MagnetometerV3 &operator=(const MagnetometerV3 &other);
	MagnetometerV3(MagnetometerV3 &&other);
	MagnetometerV3 &operator=(MagnetometerV3 &&other);

	/**
	 * @brief Инициализация манитометра
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка 
	 */
	ISL_StatusTypeDef Init() {
		return LIS2MDL::Init();
	};

	/**
	 * @brief Чтение значения с датчика во внутренний буфер
	 * 
	 */
	ISL_StatusTypeDef Read() __attribute__((deprecated("This method exists for backward compatibility and does nothing. "
														"Methods X(), Y(), Z(), RawX(), RawY(), RawZ() perform read operation on their own"))) {
		return ISL_OK;
	}

	/**
	 * @brief Получение необработанного значения по оси X
	 * 
	 * @return Необработанное значение напряжённости магнитного поля по оси X
	 */
	int16_t RawX() {
		return LIS2MDL::RawMX();
	}
	
	/**
	 * @brief Получение необработанного значения по оси Y
	 * 
	 * @return Необработанное значение напряжённости магнитного поля по оси Y
	 */
	int16_t RawY() {
		return LIS2MDL::RawMY();
	}
	
	/**
	 * @brief Получение необработанного значения по оси Z
	 * 
	 * @return Необработанное значение напряжённости магнитного поля по оси Z
	 */
	int16_t RawZ() {
		return LIS2MDL::RawMZ();
	}

	/**
	 * @brief Получение значения напряжённости по оси X
	 * 
	 * @return Значение напряжённости магнитного поля по оси X
	 */
	float X() {
		return LIS2MDL::MX();
	}
	
	/**
	 * @brief Получение значения напряжённости по оси Y
	 * 
	 * @return Значение напряжённости магнитного поля по оси Y
	 */
	float Y() {
		return LIS2MDL::MY();
	}
	
	/**
	 * @brief Получение значения напряжённости по оси Z
	 * 
	 * @return Значение напряжённости магнитного поля по оси Z
	 */
	float Z() {
		return LIS2MDL::MZ();
	}
#ifndef ARDUINO	
	// TODO @Goldfor @TeaCupMe Что тут получаем?
	Quaternion<float> GetQuaternion() {
		std::array<float, 3> buf;
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = std::atan2(RawY(), RawX());
		return from_euler(buf);
	}
#endif

	ISL_StatusTypeDef Disable() override {
		return LIS2MDL::Disable();
	}

	ISL_StatusTypeDef Enable() override {
		return LIS2MDL::Enable();
	}

	~MagnetometerV3() {};
};

} /* namespace IntroSatLib */

#endif /* MAGNETOMETERV3_H_ */
