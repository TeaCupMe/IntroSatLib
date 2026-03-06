#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC-BT986.h"

namespace IntroSatLib {
    
    ISL_StatusTypeDef FSC_BT986::writePIN(uint8_t* pinCode, uint16_t timeout) {
        return writeAT((uint8_t*)"PIN", pinCode, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::readPIN(uint8_t* rxbuff, uint16_t timeout) {
        return readAT((uint8_t*)"PIN", rxbuff, 30, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::readMAC(uint8_t* rxbuff, uint16_t timeout) {
        return readAT((uint8_t*)"ADDR", rxbuff, 28, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::connect(uint8_t* mac, uint16_t timeout) {
        return writeAT((uint8_t*)"SPPCONN", mac, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::writePrgReset(uint16_t timeout) {
        return readAT((uint8_t*)"REBOOT", NULL, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::writePrgMode(uint8_t state, uint16_t timeout) {
        return readAT((uint8_t*)"REBOOT", (state == 0) ? (uint8_t*)'0' : (uint8_t*)'1', timeout);
    }


    

    ISL_StatusTypeDef FSC_BT986::writeHardReset(uint8_t state) {
        if (_pins._reset.isValid()) {
            _pins._reset.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    ISL_StatusTypeDef FSC_BT986::writeHardMode(uint8_t state) {
        if (_pins._mode.isValid()) {
            _pins._mode.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    uint8_t FSC_BT986::readStatus() {
        _pins._status.read();
    }

    ISL_StatusTypeDef FSC_BT986::writeHardDisconnect(uint8_t state) {
        if (_pins._disconnect.isValid()) {
            _pins._disconnect.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

}

#endif
