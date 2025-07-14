#include "../GPIO.h"
#if defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO)

IntroSatLib::interfaces::GPIO::GPIO(GPIO_TypeDef* port, uint16_t pin): _port(port), _pin(pin) { }

uint8_t IntroSatLib::interfaces::GPIO::read() const
{ return HAL_GPIO_ReadPin(_port, _pin); }

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{ HAL_GPIO_WritePin(_port, _pin, state > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET); }

uint8_t IntroSatLib::interfaces::GPIO::wait(uint8_t state, uint16_t timeout) const
{
	state = !!state;
	uint32_t firstTime = HAL_GetTick();
	while(read() != state)
	{
		if ((HAL_GetTick() - firstTime) > timeout) { return 1; }
	}
	return 0;
}

#endif /* defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO) */
