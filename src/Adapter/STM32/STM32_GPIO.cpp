#include "Adapter/GPIO.h"
#if defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO)

IntroSatLib::interfaces::GPIO::GPIO(GPIO_HANDLE_TYPE pin): _pin(pin) { }

uint8_t IntroSatLib::interfaces::GPIO::read() const
{ return _pin.port ? HAL_GPIO_ReadPin(_pin.port, _pin.pin) : 0; }

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{ if (_pin.port ) HAL_GPIO_WritePin(_pin.port, _pin.pin, state > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET); }



#endif /* defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO) */
