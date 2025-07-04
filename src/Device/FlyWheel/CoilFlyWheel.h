#ifndef COILFLYWHEEL_H_
#define COILFLYWHEEL_H_

#include "BaseFlyWheel.h"

namespace IntroSatLib {

class CoilFlyWheel: public BaseFlyWheel {
public:
#ifndef ARDUINO
	/**
	 * @brief Конструктор объекта для управления магнитными катушками 
	 * @note Только в STM32CubeIDE
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес магнитных катушек на шине I2C
	 */
	CoilFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
/**
	 * @brief Конструктор объекта для управления магнитными катушками 
	 * @note Только в Arduino IDE
	 * 
	 * @param hi2c объект @b TwoWire или @b Wire 
	 * @param address адрес магнитных катушек на шине I2C
	 */
	CoilFlyWheel(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Конструктор объекта для управления магнитными катушками на @b I2C1 
	 * @note Только в Arduino IDE
	 * 
	 * @param address адрес акселлерометра на шине I2C
	 */
	CoilFlyWheel(uint8_t address = BASE_ADDRESS);
#endif
	/**
	 * @brief Конструктор объекта для управления магнитными катушками как копии другого объекта
	 * 
	 * @param other исходный объект для копирования
	 */
	CoilFlyWheel(const CoilFlyWheel &other);
	CoilFlyWheel(CoilFlyWheel &&other);
	CoilFlyWheel& operator=(const CoilFlyWheel &other);
	CoilFlyWheel& operator=(CoilFlyWheel &&other);
	~CoilFlyWheel() override;
};

} /* namespace IntroSatLib */

#endif /* COILFLYWHEEL_H_ */
