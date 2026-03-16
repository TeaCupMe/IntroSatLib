#define ISL_INTERNAL

#include "Adapter/UART.h"
#if defined(ISL_UART_ENABLED)

#include "ATDevice.h"

#if __has_include(<cstring>)
    #include <cstring>
#elif __has_include(<string.h>)
    #include <string.h>
#else
    #error "You need to write strlen() yourself :)"
#endif


namespace IntroSatLib {

    ISL_StatusTypeDef ATDevice::AT_CMD_END(uint8_t** ptr, uint8_t** end, uint8_t nargs) {
        if (*ptr + 2 >= *end) return ISL_ERROR;
        memcpy(*ptr, "\r\n", 2);
        *ptr += 2;
        
        if (*ptr >= *end) return ISL_ERROR;
        **ptr = '\0';
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_EQL(uint8_t** ptr, uint8_t** end, uint8_t nargs) {
        if (*ptr + 1 >= *end) return ISL_ERROR;
        **ptr = '=';
        *ptr += 1;
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_SEP(uint8_t** ptr, uint8_t** end, uint8_t nargs) {
        if (*ptr + 1 >= *end) return ISL_ERROR;
        **ptr = ',';
        *ptr += 1;
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_START(uint8_t** ptr, uint8_t** end, uint8_t nargs) {
        if (*ptr + 3 >= *end) return ISL_ERROR;
        memcpy(*ptr, "AT+", 3);
        *ptr += 3;
        return ISL_OK;
    }



    ISL_StatusTypeDef ATDevice::executeATCommand(uint8_t* command, uint8_t* rxbuff, uint16_t length, uint16_t timeout) {
        uint8_t buff;
        while(available()) ReadUART(&buff, 1, 10);

        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(command, strlen((const char*)command), timeout));
        
        uint16_t itter = 0;
        while (ReadUART(rxbuff + itter, 1, timeout) == ISL_OK && itter < length) itter++;

        return ISL_OK;
    }


    ISL_StatusTypeDef ATDevice::readAT(const char* cmd, uint8_t* rxbuff, uint16_t rxbuff_len, uint16_t timeout) {
        uint8_t buff[buff_size], message[buff_size];
        if (AT_CMD(message, buff_size, cmd) == ISL_OK) {
            executeATCommand(message, buff, rxbuff_len, timeout);
            if (strstr((char*)buff, AT_ERROR) != nullptr) return ISL_ERROR;

            memcpy(rxbuff, buff, rxbuff_len);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

}

#endif
