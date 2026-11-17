/*
 * MS5611.h
 *
 *  Created on: Mar 6, 2025
 *      Author: unflesh
 */

#ifndef MS5611_H_
#define MS5611_H_

#include "Device/Base/BaseBarometer.h"
#include "Device/Base/BaseTemperatureSensor.h"
#include "Device/I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

namespace IntroSatLib {

/**
 * \~english @brief This class is responsible for communicating with the MS5611 barometer via the I2C bus.
 * \~russian @brief Этот класс отвечает за взаимодействие с барометром MS5611 через шину I2C
 */
class MS5611 : public BaseBarometer, protected I2CDevice {

public:

	// Add OSR to the command REQUEST_PRESSURE or REQUEST_TEMPERATURE
	/**
	 * \~english @brief Oversampling ratio for the MS5611 barometer.
	 * \~russian @brief Коэффициент передискретизации для барометра MS5611
	 */ 
	enum OSR {
		OSR_256 = 0x00,		/**< \~english @brief Oversampling ratio 256 \~russian @brief Коэффициент передискретизации 256 */
		OSR_512 = 0x02,		/**< \~english @brief Oversampling ratio 512 \~russian @brief Коэффициент передискретизации 512 */
		OSR_1024 = 0x04,	/**< \~english @brief Oversampling ratio 1024 \~russian @brief Коэффициент передискретизации 1024 */
		OSR_2048 = 0x06,	/**< \~english @brief Oversampling ratio 2048 \~russian @brief Коэффициент передискретизации 2048 */
		OSR_4096 = 0x08		/**< \~english @brief Oversampling ratio 4096 \~russian @brief Коэффициент передискретизации 4096 */
	};

private:
	enum ERROR_TYPE: uint8_t {
		OK = ISL_OK,
		ERROR = ISL_ERROR,
		BUSY = ISL_BUSY,
		TIMEOUT = ISL_TIMEOUT,
		CRC_ERROR = 0x05,
		ADC_ERROR = 0x06
	};

	enum CMD: uint8_t {
		RST = 0x1E,
		REQUEST_PROM = 0xA0,
		REQUEST_PRESSURE = 0x40,
		REQUEST_TEMPERATURE = 0x50,
		ADC_READ = 0x00
	};

	/* Use PROM_N enum as an index for _koeff_prom array */
	enum  PROM_N: uint8_t {
		RSRV = 0, /* reserved */
		SENS_T1, /* pressure sensitivity */
		OFF_T1, /* pressure offset */
		TCS, /* temperature coefficient of pressure sensitivity */
		TCO, /* temperature coefficient of pressure offset */
		T_REF, /* reference temperature */
		TEMPSENS, /* temperature coefficient of the temperature */
		CRC_4 /* CRC-4 */
	};

	static const uint8_t BASE_ADDRESS = 0x77; /* another available address is 0x76 */
	static constexpr float P_SEA_LEVEL = 1013.25f; // mbar

	OSR _osr = OSR::OSR_256;

	uint16_t _koeff_prom[8]; /* PROM content array */
	uint8_t _crc;

	int32_t dT;
	int32_t TEMP;
	int64_t OFF;
	int64_t SENS;
	int64_t P;
	uint32_t _raw_pressure;
	uint32_t _raw_temperature;
	float _pressure;
	float _temperature;
	ISL_StatusTypeDef ReadRawPressure();
	ISL_StatusTypeDef ReadRawTemperature();
	ISL_StatusTypeDef ReadADC();
	ISL_StatusTypeDef ReadPROM();
	uint8_t CalculateCRC();
public:

	/**
	 * \~english @brief Create an instance of the MS5611 class.
	 * \~russian @brief Создание объекта класса MS5611.
	 * \~english @param i2c The I2C interface to use for communication with the barometer. Wire in Arduino IDE, I2C_HandleTypeDef in STM32CubeIDE.
	 * \~russian @param i2c Интерфейс I2C, используемый для взаимодействия с барометром. Wire в Arduino IDE, I2C_HandleTypeDef в STM32CubeIDE.
	 */
	MS5611(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);

#ifdef DEBUG
	void GetPROM(uint16_t* buffer);
#endif

	/**
	 * \~english @brief Initialize the MS5611 barometer.
	 * \~russian @brief Инициализация барометра MS5611.
	 * \~english @return The status of the initialization. ISL_OK if successful.
	 * \~russian @return Статус выполнения метода ISL. ISL_OK при успешной инициализации.
	 */
	ISL_StatusTypeDef Init(OSR osr);

	/**
	 * \~english @brief Initialize the MS5611 barometer with default oversampling ratio.
	 * \~russian @brief Инициализация барометра MS5611 с коэффициентом передискретизации по умолчанию.
	 * \~english @return The status of the initialization. ISL_OK if successful.
	 * \~russian @return Статус выполнения метода ISL. ISL_OK при успешной инициализации.
	 */
	ISL_StatusTypeDef Init() override {
		return Init(OSR::OSR_2048);
	}

	/**
	 * \~english @brief Set the oversampling ratio for the MS5611 barometer.
	 * \~russian @brief Установить коэффициент передискретизации для барометра MS5611.
	 * \~english @param osr The oversampling ratio to set.
	 * \~russian @param osr Коэффициент передискретизации для установки.
	 * \~english @return The status of the operation. ISL_OK if successful.
	 * \~russian @return Статус выполнения метода ISL. ISL_OK при успешной установке.
	 */
	ISL_StatusTypeDef SetOSR(OSR osr);


	/**
	 * \~english @brief Get the pressure.
	 * \~russian @brief Получить давление.
	 * \~english @return The pressure value in mbar.
	 * \~russian @return Значение давления в мбар.
	 */
	float GetPressure() override; // TODO: if error return NaN

	/**
	 * \~english @brief Get the temperature.
	 * \~russian @brief Получить температуру.
	 * \~english @return The temperature value in degrees Celsius.
	 * \~russian @return Значение температуры в градусах Цельсия.
	 */
	float GetTemperature();

	/**
	 * \~english @brief Get the height.
	 * \~russian @brief Получить высоту.
	 * \~english @return The height value in meters.
	 * \~russian @return Значение высоты в метрах.
	 */
	float GetHeight();

	~MS5611();
};

} /* namespace IntroStratLib */

#endif /* MS5611_H_ */
