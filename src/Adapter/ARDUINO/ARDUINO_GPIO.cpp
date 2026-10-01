#include "Adapter/GPIO.h"
#if defined(ARDUINO)

IntroSatLib::interfaces::GPIO::GPIO(GPIO_HANDLE_TYPE pin): _pin(pin) { }

uint8_t IntroSatLib::interfaces::GPIO::read() const
{ return digitalRead(_pin); }

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{ digitalWrite(_pin, state > 0 ? HIGH : LOW); }

bool IntroSatLib::interfaces::GPIO::isValid() const 
{ return _pin != 0; }

void IntroSatLib::interfaces::GPIO::tone(uint16_t frequency)
{ ::tone(_pin, frequency); }

void IntroSatLib::interfaces::GPIO::noTone()
{ ::noTone(_pin);}

#endif /* defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO) */
