#define INTROSATLIB_INTERNAL
#include "Adapter/UART.h"

#if defined(ARDUINO) // If HAL SPI is enabled and no Arduino framework

uint8_t IntroSatLib::interfaces::UART::available() {
    return _huart->available(); // TODO implement for HAL??
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::receive(uint8_t* rx_buf, uint16_t count, uint16_t timeout) {
    uint32_t finishTime = system::GetTick() + timeout;

    while (finishTime < system::GetTick()) {
        if (available() >= count) {
            if (_huart->readBytes(rx_buf, count) == count) {
                return ISL_OK;
            } else {
                return ISL_ERROR;
            }
        }
    }
    return ISL_TIMEOUT;
    
    // Алтернативно - тут мы пишем в пользовательский буффер даже если принято недостаточно данных, выглядит не очень
    // _huart->setTimeout(timeout);
	// return (_huart->readBytes(rx_buf, count) == count);
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmit(uint8_t* tx_buf, uint16_t count, uint16_t timeout) {
    _huart->setTimeout(timeout);
    return (ISL_StatusTypeDef) HAL_UART_Transmit(_huart, txbuf, count, timeout);
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmitAsync(uint8_t* tx_buf, uint16_t count) {
    _huart->setTimeout(timeout);
    return (ISL_StatusTypeDef) HAL_UART_Transmit_IT(_huart, txbuf, count);
}

#endif /* defined(HAL_UART_MODULE_ENABLED) && !defined(ARDUINO) */