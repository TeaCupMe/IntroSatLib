#ifndef ADAPTER_I2C_I2CADAPTER_H_
#define ADAPTER_I2C_I2CADAPTER_H_

#include "Adapter/I2C/I2C.h"

#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include "Wire.h"
	#define ISL_I2C_ENABLED
	namespace IntroSatLib {
	namespace interfaces {
	using I2C_HANDLE_TYPE = TwoWire;
	} /* namespace interfaces */
	} /* namespace IntroSatLib */

#else
/*********************************/
/************ Native *************/
/*********************************/
	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series outside Arduino IDE
		#error "Bare AVR outside of Arduino IDE is not yet supported"

	#elif defined(INTROSATLIB_HOST_UNIT_TESTS)
	/***** Host unit tests (MockI2C DI) ******/
		// No HAL / Wire: enable the I2C API surface for driver headers.
		// Handle type is opaque; handle ctors are omitted in .cpp under this flag.
		#define ISL_I2C_ENABLED
		namespace IntroSatLib {
		namespace interfaces {
		struct I2C_HANDLE_TYPE;
		} /* namespace interfaces */
		} /* namespace IntroSatLib */

	#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
	/*****  STM32 and stm32duino ******/
		#include "Adapter/STM32/stm32xxyy_hal.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL configuration incorrect"
		#endif

		#ifdef HAL_I2C_MODULE_ENABLED
			#define ISL_I2C_ENABLED
			namespace IntroSatLib {
			namespace interfaces {
			using I2C_HANDLE_TYPE = I2C_HandleTypeDef;
			} /* namespace interfaces */
			} /* namespace IntroSatLib */
		#elif !defined(ISL_INTERNAL)
			#error "I2C not enabled as part of HAL"
		#endif

	// #elif defined(AMUR)
	/************** AMUR *************/
		// #error "AMUR not yet supported"

	#else
	/************ UNKNOWN ************/
		#ifndef ISL_INTERNAL
			#error Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros.  \
			 		Currently supported systems are: stm32 with HAL, stm32duino. AVR planned for future support.
		#endif
	#endif
#endif /* ARDUINO */


#ifdef ISL_I2C_ENABLED

#define ASSERT_I2C_HAVE() \
if(!_hi2c) { \
	logText("Haven't i2c handle"); \
	return ISL_StatusTypeDef::ISL_ERROR; \
}

#define LOG_I2C_ADDRESS() \
logText("Device in "); \
logHEX(deviceAddress);

#if LOGDATA
#define LOG_I2C_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_I2C_BUFFER(Sep, Data, Nbytes)
#endif


namespace IntroSatLib {
namespace interfaces {

/**
 * Library-internal platform Adapter (Arduino Wire / STM32 HAL).
 * End users pass hi2c / Wire into drivers; they do not construct this type.
 */
class I2CAdapter final : public I2C {
	enum class I2CSpeed
	{
		Standard = 0,
		Fast = 1
	};
private:
	/** Handle-keyed shared Adapter pool (defined in I2CAdapterPool.cpp). */
	struct Pool; // incomplete here; Pool::Slot holds I2CAdapter values
	static Pool pool_;

	I2C_HANDLE_TYPE *_hi2c = 0;
	I2CSpeed _speed = I2CSpeed::Standard;
	ISL_StatusTypeDef innerIsReady(uint8_t address);
public:

	/**
	 * @brief Shared Adapter for a platform handle (Wire / HAL).
	 * Returns a reference to a process-wide pool slot keyed by handle address.
	 * Multiple devices on the same bus share one Adapter instance.
	 */
	static I2C& from(I2C_HANDLE_TYPE& hi2c);
	static I2C& from(I2C_HANDLE_TYPE* hi2c);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2CAdapter(I2C_HANDLE_TYPE *hi2c);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2CAdapter(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2CAdapter(I2C_HANDLE_TYPE &hi2c): I2CAdapter(&hi2c) {};

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2CAdapter(I2C_HANDLE_TYPE &hi2c, I2CSpeed speed): I2CAdapter(&hi2c, speed) {};

	I2CAdapter(const I2CAdapter& other);
	I2CAdapter(I2CAdapter&& other);
	I2CAdapter& operator=(const I2CAdapter& other);
	I2CAdapter& operator=(I2CAdapter&& other);

	/**
	 * @brief Проверка доступности устройства на шине к работе
	 *
	 * @param waitIsReady
	 * @returns 0, если устройство доступно
	 * @returns 1, если с устройством произошла ошибка
	 */
	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0) override;

	/**
	 * @brief Отправка запроса на чтение Nbytes байт с устройства
	 *
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в которую будет записан результат. В случае
	 * @param[in] Nbytes Количество байт  для чтения
	 * @return Статус выполнения чтения
	 */
	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override;

	/**
	 * @brief Отправка запроса на чтение Nbytes байт из регистра Register устройства
	 *
	 * @param[in] Register Адрес регистра для чтения
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в который будет записан результат. В случае
	 * @param[in] Nbytes Количество байт для чтения
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes) override;

	/**
	 * @brief Отправка запроса на запись NBytes байт на устройство
	 *
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) override;

	/**
	 * @brief Отправка запроса на запись NBytes байт в регистр Register устройства
	 *
	 * @param Register Адрес регистра для записи
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes) override;
	~I2CAdapter() override;
};

} /* namespace interfaces */
} /* namespace IntroSatLib */
#endif /* ISL_I2C_ENABLED */
#endif /* ADAPTER_I2C_I2CADAPTER_H_ */
