#define ISL_INTERNAL

#include "Adapter/UART.h"
#if defined(ISL_UART_ENABLED)

#include "UARTDevice.h"

namespace IntroSatLib {

    ISL_StatusTypeDef UARTDevice::WriteUART(uint8_t* buf, uint8_t nBytes, uint16_t timeout) {
        return _uart.transmit(buf, nBytes, timeout);
    }

    ISL_StatusTypeDef UARTDevice::ReadUART(uint8_t* buf, uint8_t nBytes, uint16_t timeout) {
        return _uart.receive(buf, nBytes, timeout);
    }

    uint8_t UARTDevice::Available() {
        return _uart.available();
    }

}

#endif
