//#ifndef STM32_I2C_H_
//#define STM32_I2C_H_
//
//#include "../I2C.h"
//
//#ifdef HAL_I2C_MODULE_ENABLED // If I2C is enabled or at least accessible (in case of Arduino IDE)
//#define I2C_ENABLED
//
//#include <array>
//
//namespace IntroSatLib {
//namespace interfaces {
//
//class I2C final {
//	enum class I2CSpeed
//	{
//		Standard = 0,
//		Fast = 1
//	};
//private:
//	I2CSpeed _speed = I2CSpeed::Standard;
//	I2C_HandleTypeDef *_hi2c = 0;
//	ISL_StatusTypeDef innerIsReady(uint8_t address);
//public:
//	/**
//	 * @note Только в STM32CubeIDE
//	 * @brief Конструктор объекта устройства на шине I2C
//	 *
//	 * @param hi2c объект @b I2C_HandleTypeDef
//	 * @param address адрес устройства на шине I2C
//	 */
//	I2C(I2C_HandleTypeDef *hi2c);
//
//	/**
//	 * @note Только в STM32CubeIDE
//	 * @brief Конструктор объекта устройства на шине I2C
//	 *
//	 * @param hi2c объект @b I2C_HandleTypeDef
//	 * @param address адрес устройства на шине I2C
//	 * @param speed скорость I2C
//	 */
//	I2C(I2C_HandleTypeDef *hi2c, I2CSpeed speed);
//	I2C(const I2C& other);
//	I2C(I2C&& other);
//	I2C& operator=(const I2C& other);
//	I2C& operator=(I2C&& other);
//
//	/**
//	 * @brief Проверка доступности устройства на шине к работе
//	 *
//	 * @param waitIsReady
//	 * @returns 0, если устройство доступно
//	 * @returns 1, если с устройством произошла ошибка
//	 */
//	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0);
//
//	/**
//	 * @brief Отправка запроса на чтение Nbytes байт с устройства
//	 *
//	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в которую будет записан результат. В случае
//	 * @param[in] Nbytes Количество байт  для чтения
//	 * @return Статус выполнения чтения
//	 */
//	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);
//
//	/**
//	 * @brief Отправка запроса на чтение Nbytes байт из регистра Register устройства
//	 *
//	 * @param[in] Register Адрес регистра для чтения
//	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в который будет записан результат. В случае
//	 * @param[in] Nbytes Количество байт для чтения
//	 * @return HAL_StatusTypeDef
//	 */
//	ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);
//
//	/**
//	 * @brief Отправка запроса на запись NBytes байт на устройство
//	 *
//	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
//	 * @param Nbytes Количество байт для записи
//	 * @return HAL_StatusTypeDef
//	 */
//	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);
//
//	/**
//	 * @brief Отправка запроса на запись NBytes байт в регистр Register устройства
//	 *
//	 * @param Register Адрес регистра для записи
//	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
//	 * @param Nbytes Количество байт для записи
//	 * @return HAL_StatusTypeDef
//	 */
//	ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);
//	~I2C();
//};
//
//} /* namespace intefaces */
//} /* namespace IntroSatLib */
//#endif /* HAL_I2C_MODULE_ENABLED */
//#endif /* STM32_I2C_H_ */
