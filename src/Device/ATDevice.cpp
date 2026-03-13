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

    template<typename... Args>
    ISL_StatusTypeDef ATDevice::AT_CMD(uint8_t* buff, size_t buff_size, const char* cmd, Args... args) {
        uint8_t* ptr = buff;
        uint8_t* end = buff + buff_size;
        constexpr uint8_t num_args = sizeof...(args);
        
        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_START(ptr, end, num_args));
        
        size_t cmd_len = strlen(cmd);
        if (ptr + cmd_len >= end) return ISL_ERROR;
        memcpy(ptr, cmd, cmd_len);
        ptr += cmd_len;

        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_EQL(ptr, end, num_args));
        if (num_args > 0) {
            
            const char* params[] = { reinterpret_cast<const char*>(args)... };
            for (uint8_t i = 0; i < num_args; i++) {
                if (i > 0) 
                    RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_SEP(ptr, end, num_args));
                
                size_t param_len = strlen(params[i]);
                if (ptr + param_len >= end) return ISL_ERROR;
                memcpy(ptr, params[i], param_len);
                ptr += param_len;
            }
        }

        return AT_CMD_END(ptr, end, num_args);
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_END(uint8_t* ptr, uint8_t* end, uint8_t nargs) {
        if (ptr + 2 >= end) return ISL_ERROR;
        memcpy(ptr, "\r\n", 2);
        ptr += 2;
        
        if (ptr >= end) return ISL_ERROR;
        *ptr = '\0';
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_EQL(uint8_t* ptr, uint8_t* end, uint8_t nargs) {
        if (nargs > 0) {
            if (ptr + 1 >= end) return ISL_ERROR;
                *ptr++ = '=';
        }
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_SEP(uint8_t* ptr, uint8_t* end, uint8_t nargs) {
        if (ptr + 1 >= end) return ISL_ERROR;
        *ptr++ = ',';
        return ISL_OK;
    }

    ISL_StatusTypeDef ATDevice::AT_CMD_START(uint8_t* ptr, uint8_t* end, uint8_t nargs) {
        if (ptr + 3 >= end) return ISL_ERROR;
        memcpy(ptr, "AT+", 3);
        ptr += 3;
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


    //TODO: many args
    ISL_StatusTypeDef ATDevice::writeAT(uint8_t* cmd, uint8_t* data, uint16_t timeout) {
        uint8_t buff[buff_size], message[buff_size];
        if (data != nullptr) {
            RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD(message, buff_size, (const char*)cmd, data))
        } else {
            RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD(message, buff_size, (const char*)cmd)) 
        }
        RETURN_STATUS_IF_NOT_OK_SILENT(executeATCommand(message, buff, buff_size, timeout));
        return (strstr((char*)buff, AT_OK) != nullptr) ? ISL_OK : ISL_ERROR;
    }


    ISL_StatusTypeDef ATDevice::readAT(uint8_t* cmd, uint8_t* rxbuff, uint16_t rxbuff_len, uint16_t timeout) {
        uint8_t buff[buff_size], message[buff_size];
        if (AT_CMD(message, buff_size, (const char*)cmd) == ISL_OK) {
            executeATCommand(message, buff, rxbuff_len, timeout);
            if (strstr((char*)buff, AT_ERROR) != nullptr) return ISL_ERROR;

            memcpy(rxbuff, buff, rxbuff_len);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

}

#endif
