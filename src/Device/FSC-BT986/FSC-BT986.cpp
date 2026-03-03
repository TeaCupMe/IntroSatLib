#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC-BT986.h"
#include <stdarg.h>
#include <string.h>
#include "Commands.h"

namespace IntroSatLib {

    uint16_t FSC_BT986::executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length, uint16_t timeout) {
        uint8_t buff;
        while(_uart->available()) _uart->receive(&buff, 1, 10);

        _uart->transmit(command, strlen((const char*)command), 100);
        
        uint16_t itter = 0;
        uint32_t timer = system::GetTick();
        while (_uart->receive(rxbuffer + itter, 1, timeout) == ISL_OK) itter++;

        return itter;
    }


    ISL_StatusTypeDef FSC_BT986::writePIN(uint8_t* pinCode, uint16_t timeout) {
        uint8_t buff[6];
        executeATCommand(AT_CMD("PIN", pinCode), buff, 6, timeout);
        return (strcmp(AT_OK, (char*)buff) == 0) ? ISL_OK : ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::readPIN(uint8_t* rxbuffer, uint16_t timeout) {
        uint8_t buff[30];
        executeATCommand((uint8_t*)AT_CMD("PIN"), buff, sizeof(buff), timeout);
        memcpy(rxbuffer, buff, sizeof(buff));
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::readMAC(uint8_t* rxbuffer, uint16_t timeout) {
        executeATCommand((uint8_t*)AT_CMD("AT+ADDR"), rxbuffer, 24, timeout);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::connect(uint8_t* mac, uint16_t timeout) {
        uint8_t buff[6];
        executeATCommand((uint8_t*)AT_CMD("SPPCONN", mac), buff, 6, timeout);
        return (strcmp(AT_OK, (char*)buff) == 0) ? ISL_OK : ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::writePrgReset(uint16_t timeout) {
        uint8_t buff[6];
        executeATCommand((uint8_t*)AT_CMD("REBOOT"), buff, 6, timeout);
        return (strcmp(AT_OK, (char*)buff) == 0) ? ISL_OK : ISL_ERROR;
    }

    // ISL_StatusTypeDef FSC_BT986::writeHardReset(uint8_t state) {
    //     if (_pins._reset != NULL) {
    //         _pins._reset.write(state);
    //         return ISL_OK;
    //     } else {
    //         return ISL_ERROR;
    //     }
    // }
}


#endif