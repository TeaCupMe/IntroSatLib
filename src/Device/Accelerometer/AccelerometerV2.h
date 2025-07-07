#ifndef ACCELEROMETERV2_H_
#define ACCELEROMETERV2_H_

#include "../I2CDevice.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {

class AccelerometerV2: private I2CDevice {

private:
  
	static const uint8_t BASE_ACCEL_CONF = 0xA8;
	static const uint8_t BASE_ADDRESS = 0x6B;
	static constexpr float _rawg = 16384.0;
    
	enum RegisterMap
	{
		WHO_AM_I_REG = 0X0F,
		CTRL1_XL = 0X10,
		CTRL8_XL = 0X17,
		OUT_TEMP_L = 0X20,
		OUT_TEMP_H = 0X21,
		OUTX_L_XL = 0X28,
		OUTX_H_XL = 0X29,
		OUTY_L_XL = 0X2A,
		OUTY_H_XL = 0X2B,
		OUTZ_L_XL = 0X2C,
		OUTZ_H_XL = 0X2D,
    };

public:
	/**
	 * @brief Диапазон измерения ускорения
	 * 
	 */
	enum Scale
	{
		twoG = 0,  		/**< Диапазон &plusmn;  2g */
		fourG = 2, 		/**< Диапазон &plusmn;  4g */
		eightG = 3,		/**< Диапазон &plusmn;  8g */
		sixteenG = 1 	/**< Диапазон &plusmn;  16g */
	};

	/**
	 * @brief Частота внутреннего фильтра измерения ускорения
	 * 
	 */
	enum FilterBandwidth
	{
		F400H = 0, /**< Частота 400 Гц */
		F200H = 1, /**< Частота 200 Гц */
		F100H = 2, /**< Частота 100 Гц */
		F050H = 3  /**< Частота 50 Гц */
	};


	/**
	 * @par <!-- @irongamer54 , что это на самом деле-->
	 * @brief Скорость обновления данных
	 * 
	 */

	// TODO @TeaCupMe я бы привёл к стилю, что всё в герцах - DR1K66H, DR3K33H и так далее. Спорный моментик
	enum DataRate
	{
		DR12_5H = 1,      /**< 12.5 Гц */
		DR26H = 2,        /**< 26 Гц */
		DR52H = 3,        /**< 52 Гц */
		DR104H = 4,       /**< 104 Гц */
		DR208H = 5,       /**< 208 Гц */
		DR416H = 6,       /**< 416 Гц */
		DR833H = 7,       /**< 833 Гц */
		DR1_66KH = 8,     /**< 1.66 кГц */
		DR3_33KH = 9,     /**< 3.33 кГц */
		DR6_66KH = 10,    /**< 6.66 кГц */
    };

    uint8_t _sens = 0;

	/**
	 * @brief Конструктор объекта акселлерометра. 
	 * @note Только в STM32CubeIDE
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес акселлерометра на шине I2C
	 */
    AccelerometerV2(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Инициализация акселлерометра с параметрами по умолчанию: @ref Scale::fourG, @ref FilterBandwidth::F400H, @ref DataRate::DR6_66KH.
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
    ISL_StatusTypeDef Init();

	/**
	 * @brief Инициализация акселлерометра с заданным диапазоном измерения
	 * 
	 * @param sens Значение чуствительности @ref Scale
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	//TODO Переименовать sens в scale, иначе вызывает путаницу. Это именно scale
    ISL_StatusTypeDef Init(Scale sens);

	/**
	 * @brief Инициализация акселлерометра с заданными диапазоном измерения и шириной окна фильтра 
	 * 
	 * @param sens Значение чуствительности @ref Scale
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	//TODO Переименовать sens в scale, иначе вызывает путаницу. Это именно scale
    ISL_StatusTypeDef Init(Scale sens, FilterBandwidth filter);

	/**
	 * @brief Инициализация акселлерометра с заданными диапазоном измерения, шириной окна фильтра и скоростью обновления данных 
	 * 
	 * @param sens Значение чуствительности @ref Scale
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 * @param datarate Значение скорости обновления данных @ref DataRate
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	//TODO Переименовать sens в scale, иначе вызывает путаницу. Это именно scale
    ISL_StatusTypeDef Init(Scale sens, FilterBandwidth filter, DataRate datarate);

	/**
	 * @brief Установка диапазона измерения
	 * 
	 * @param sens Значение чуствительности @ref Scale
	 */
	//TODO Переименовать sens в scale, иначе вызывает путаницу. Это именно scale
	//? @irongamer54 Тут параметр по умолчанию Scale::twoG, хотя в Init() выставляется fourG (из BASE_ACCELL_CONF)
    ISL_StatusTypeDef SetScale(Scale sens = twoG);

	/**
	 * @brief Установка ширины окна фильтра
	 * 
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 */
    ISL_StatusTypeDef SetFilter(FilterBandwidth filter = F400H);

	/**
	 * @brief Установка скорости обновления данных
	 * 
	 * @param datarate datarate Значение скорости обновления данных @ref DataRate
	 */
    ISL_StatusTypeDef SetDataRate(DataRate datarate);

	/**
	 * @brief Отключение акселлерометра - переход в режим экономии энергии
	 * 
	 */
    ISL_StatusTypeDef end();

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
	 * @brief Получение значения ускорения по оси X.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float X();
    
	/**
	 * @brief Получение значения ускорения по оси Y.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Y();
    
	/**
	 * @brief Получение значения ускорения по оси Z.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Z();

	/**
	 * @brief Получение значения температуры акселлерометра
	 * 
	 * @return Значение температуры в &deg;C
	 */
    float Temp();

    virtual ~AccelerometerV2();
};

} /* namespace IntroSatLib */

#endif /* ACCELEROMETER_H_ */
