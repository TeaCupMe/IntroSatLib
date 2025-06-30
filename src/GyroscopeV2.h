#ifndef GYROSCOPE_V2_H_
#define GYROSCOPE_V2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#endif

namespace IntroSatLib {

class GyroscopeV2: public BaseDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x6B;
	static constexpr float _rawdps = (8.75f / 1000.f) * 3.1415926f / 180.0f;

	enum RegisterMap
	{
		GYRO_CONFIG = 0X11,
		GYRO_XOUT_H = 0X22,
		GYRO_XOUT_L,
		GYRO_YOUT_H,
		GYRO_YOUT_L,
		GYRO_ZOUT_H,
		GYRO_ZOUT_L,
	};

	static float cutMin(float value, float cut);

	uint8_t _sensitivity = 0;
	uint8_t _dataRate = 0;

	uint32_t _lastXTime = 0;
	uint32_t _lastYTime = 0;
	uint32_t _lastZTime = 0;

	float _cutX = 0.0872665;
	float _cutY = 0.0872665;
	float _cutZ = 0.0872665;

	float _lastX = 0;
	float _lastY = 0;
	float _lastZ = 0;

public:

	/**
	 * @brief Скорость обновления данных
	 * 
	 */
	enum DataRate
	{
		OFF,		/**< Автоматическое определение */
		F_13_Hz,	/**< 13 Гц */
		F_26_Hz,	/**< 26 Гц */
		F_52_Hz,	/**< 52 Гц */
		F_104_Hz,	/**< 104 Гц */
		F_208_Hz,	/**< 208 Гц */
		F_416_Hz,	/**< 416 Гц */
		F_833_Hz,	/**< 833 Гц */
		F_1666_Hz,	/**< 1666 Гц */
	};

	/**
	 * @brief Диапазон измерения
	 * 
	 */
	enum Scale
	{
		DPS0250 = 0,	/**< Диапазон &plusmn;250 град/с */
		DPS0500,		/**< Диапазон &plusmn;500 град/с */
		DPS1000,		/**< Диапазон &plusmn;1000 град/с */
		DPS2000			/**< Диапазон &plusmn;2000 град/с */
	};

#ifndef ARDUINO
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта гироскопа. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес гироскопа на шине I2C
	 */
	GyroscopeV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	/**
	 * @note Только в Arduino IDE
	 * @brief Конструктор объекта гироскопа
	 * 
	 * @param hi2c объект @b TwoWire или @b Wire 
	 * @param address адрес гироскопа на шине I2C
	 */
	GyroscopeV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	
	/**
	 * @note Только в Arduino IDE
	 * @brief Конструктор объекта гироскопа на @b I2C1 
	 * 
	 * @param address адрес гироскопа на шине I2C
	 */
	GyroscopeV2(uint8_t address = BASE_ADDRESS);
#endif
	/**
	 * @brief Конструктор объекта гироскопа как копии другого объекта гироскопа
	 * 
	 * @param other исходный объект для копирования
	 */
	GyroscopeV2(const GyroscopeV2 &other);
	GyroscopeV2(GyroscopeV2 &&other);
	GyroscopeV2& operator=(const GyroscopeV2 &other);
	GyroscopeV2& operator=(GyroscopeV2 &&other);

	/**
	 * @brief Инициализация гироскопа с параметрами по умолчанию: @ref DataRate::F_104_Hz, @ref Scale::DPS0250
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка 
	 */
	uint8_t Init() override;

	/**
	 * @brief Инициализация гироскопа с заданным диапазоном измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	uint8_t Init(Scale sensitivity);

	/**
	 * @brief Инициализация гироскопа с заданным диапазоном измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @param datarate Значение скорости обновления данных @ref DataRate
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	uint8_t Init(Scale sensitivity, DataRate dataRate);

	/**
	 * @brief Установка диапазона измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	void SetScale(Scale sensitivity);

	/**
	* @brief Установка скорости обновления данных
	* 
	* @param datarate Значение скорости обновления данных @ref DataRate
	*/
	void SetDataRate(DataRate dataRate);
public:
	/**
	 * @brief Получение @b необработанного значения ускорения по оси X.
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawX();

	/**
	 * @brief Получение @b необработанного значения ускорения по оси Y. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawY();

	/**
	 * @brief Получение @b необработанного значения ускорения по оси Z. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawZ();

	/**
	 * @brief Получение значения ускорения вокруг оси X.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */	
	float X();

	/**
	 * @brief Получение значения ускорения вокруг оси Y.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Y();

	/**
	 * @brief Получение значения ускорения вокруг оси Z.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */	
	float Z();

	/**
	 * @brief Получение углового перемещения вокруг оси X с момента предыдущего вызова @ref integrationX()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationX();
	
	/**
	 * @brief Получение углового перемещения вокруг оси Y с момента предыдущего вызова @ref integrationY()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationY();
	
	/**
	 * @brief Получение углового перемещения вокруг оси Z с момента предыдущего вызова @ref integrationZ()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationZ();

	/**
	 * @brief Установка порогового значения по оси X
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref X() вернёт 0.
	 * @param x Пороговое значение 
	 */
	void SetMinCutX(float x);

	/**
	 * @brief Установка порогового значения по оси Y
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref Y() вернёт 0.
	 * @param y Пороговое значение
	 */
	void SetMinCutY(float y);

	/**
	 * @brief Установка порогового значения по оси Z
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref Z() вернёт 0.
	 * @param z Пороговое значение
	 */
	void SetMinCutZ(float z);
#ifndef ARDUINO
	// TODO @TeaCupMe Описать более понятным языком
	/**
	 * @brief Получение изменения углового положения в виде набора кватернионов
	 * @note Метод использует @ref IntegrationX(), @ref IntegrationY() и @ref IntegrationZ(). 
	 * В связи с этим, для получения достоверных данных необходимо вызывать достаточно часто.
	 * @remark Только в STM32CubeIDE
	 * @return Набор кваретнионов, описывающий изменение углового положения
	 */
	Quaternion<float> GetQuaternion();
#endif
	~GyroscopeV2() override;
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_V2_H_ */
