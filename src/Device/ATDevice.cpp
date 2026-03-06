#define ISL_INTERNAL

#include "Adapter/UART.h"
#if defined(ISL_UART_ENABLED)

#include "ATDevice.h"
#include <cstring>
#include <string>

namespace IntroSatLib {

    template<typename... Args>
    ISL_StatusTypeDef ATDevice::AT_CMD(uint8_t* buffer, size_t buffer_size, const char* cmd, Args... args) {
        uint8_t* ptr = buffer;
        uint8_t* end = buffer + buffer_size;
        
        if (ptr + 3 >= end) return ISL_ERROR;
        memcpy(ptr, "AT+", 3);
        ptr += 3;
        
        size_t cmd_len = strlen(cmd);
        if (ptr + cmd_len >= end) return ISL_ERROR;
        memcpy(ptr, cmd, cmd_len);
        ptr += cmd_len;
        
        constexpr uint8_t num_args = sizeof...(args);
        if (num_args > 0) {
            if (ptr + 1 >= end) return ISL_ERROR;
            *ptr++ = '=';
            
            const char* params[] = { reinterpret_cast<const char*>(args)... };
            for (uint8_t i = 0; i < num_args; i++) {
                if (i > 0) {
                    if (ptr + 1 >= end) return ISL_ERROR;
                    *ptr++ = ',';
                }
                
                size_t param_len = strlen(params[i]);
                if (ptr + param_len >= end) return ISL_ERROR;
                memcpy(ptr, params[i], param_len);
                ptr += param_len;
            }
        }
        
        if (ptr + 2 >= end) return ISL_ERROR;
        memcpy(ptr, "\r\n", 2);
        ptr += 2;
        
        if (ptr >= end) return ISL_ERROR;
        *ptr = '\0';
        
        return ISL_OK;
    }

    uint16_t ATDevice::executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length, uint16_t timeout) {
        uint8_t buff;
        while(available()) ReadUART(&buff, 1, 10);

        WriteUART(command, strlen((const char*)command), 100);
        
        uint16_t itter = 0;
        while (ReadUART(rxbuffer + itter, 1, timeout) == ISL_OK && itter < length) itter++;

        return itter;
    }


    ISL_StatusTypeDef ATDevice::writeAT(uint8_t* cmd, uint8_t* data, uint16_t timeout) {
        uint8_t buff[6], message[buff_size];
        if (data != NULL) {
            if (AT_CMD(message, buff_size, (const char*)cmd, data) == ISL_OK) 
                executeATCommand(message, buff, 6, timeout);
        } else {
            if (AT_CMD(message, buff_size, (const char*)cmd) == ISL_OK) 
                executeATCommand(message, buff, 6, timeout);
        }
        return (strcmp(AT_OK, (char*)buff) == 0) ? ISL_OK : ISL_ERROR;
    }


    ISL_StatusTypeDef ATDevice::readAT(uint8_t* cmd, uint8_t* rxbuffer, uint16_t rxbuffer_len, uint16_t timeout) {
        uint8_t buff[buff_size], message[buff_size];
        if (AT_CMD(message, buff_size, (const char*)cmd) == ISL_OK) {
            uint8_t len = executeATCommand(message, buff, sizeof(buff), timeout);
            if (strstr((char*)buff, AT_ERROR) != NULL) return ISL_ERROR;

            memcpy(rxbuffer, buff, len < rxbuffer_len ? len : rxbuffer_len);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

}

#endif
