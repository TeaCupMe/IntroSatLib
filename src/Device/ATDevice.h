#ifndef ATDEVICE_H_
#define ATDEVICE_H_

#include "Device/UARTDevice.h"

#if __has_include(<cstring>)
    #include <cstring>
#elif __has_include(<string.h>)
    #include <string.h>
#else
    #error "You need to write strlen() yourself :)"
#endif

#define AT_OK       "\r\nOK\r\n"
#define AT_ERROR    "\r\nERROR\r\n"


namespace IntroSatLib {

class ATDevice: public UARTDevice {
private:

protected:

    uint16_t buff_size;
    static constexpr uint16_t DEFAULT_TIMEOUT = 100;
    static constexpr uint16_t DEFAULT_BSIZE = 128;

    virtual ISL_StatusTypeDef AT_CMD_END(uint8_t** ptr, uint8_t** end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_SEP(uint8_t** ptr, uint8_t** end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_EQL(uint8_t** ptr, uint8_t** end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_START(uint8_t** ptr, uint8_t** end, uint8_t nargs);

    ISL_StatusTypeDef executeATCommand(uint8_t* command, 
                                        uint8_t* rxbuff, 
                                        uint16_t length, 
                                        uint16_t timeout=DEFAULT_TIMEOUT);

public:

    ATDevice(interfaces::UART uart, 
            const uint16_t bsize = DEFAULT_BSIZE) : UARTDevice(uart), 
                                                    buff_size(bsize) {}

    ISL_StatusTypeDef readAT(const char* cmd, uint8_t* rxbuff, 
                            uint16_t rxbuff_len = DEFAULT_BSIZE, 
                            uint16_t timeout=DEFAULT_TIMEOUT);

    template<typename... Args>
    ISL_StatusTypeDef writeAT(const char* cmd, uint16_t timeout, Args... args) {
        uint8_t buff[buff_size], message[buff_size];

        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD(message, buff_size, cmd, args...))
        
        RETURN_STATUS_IF_NOT_OK_SILENT(executeATCommand(message, buff, buff_size, timeout));
        return (strstr((char*)buff, AT_OK) != nullptr) ? ISL_OK : ISL_ERROR;
    }

    template<typename... Args>
    ISL_StatusTypeDef writeAT(const char* cmd, Args... args) {
        return writeAT(cmd, DEFAULT_TIMEOUT, args...);
    }

    template<typename... Args>
    ISL_StatusTypeDef AT_CMD(uint8_t* buff, uint8_t buff_size, const char* cmd, Args... args) {
        uint8_t* ptr = buff;
        uint8_t* end = buff + buff_size;
        constexpr uint8_t num_args = sizeof...(args);
        
        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_START(&ptr, &end, num_args));
        
        uint8_t cmd_len = strlen(cmd);
        if (ptr + cmd_len >= end) return ISL_ERROR;
        memcpy(ptr, cmd, cmd_len);
        ptr += cmd_len;

        if (num_args > 0) {
            RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_EQL(&ptr, &end, num_args));
            
            const char* params[] = { reinterpret_cast<const char*>(args)... };
            for (uint8_t i = 0; i < num_args; i++) {
                if (i > 0) 
                    RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_SEP(&ptr, &end, num_args));
                
                uint8_t param_len = strlen(params[i]);
                if (ptr + param_len >= end) return ISL_ERROR;
                memcpy(ptr, params[i], param_len);
                ptr += param_len;
            }
        }

        return AT_CMD_END(&ptr, &end, num_args);
    }

};

}

#endif
