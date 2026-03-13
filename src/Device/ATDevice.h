#ifndef ATDEVICE_H_
#define ATDEVICE_H_

#include "Device/UARTDevice.h"

#define AT_OK       "\r\nOK\r\n"
#define AT_ERROR    "\r\nERROR\r\n"


namespace IntroSatLib {

class ATDevice: public UARTDevice {
private:

protected:

    uint16_t buff_size;

    template<typename... Args>
    ISL_StatusTypeDef AT_CMD(uint8_t* buff, size_t buff_size, const char* cmd, Args... args);

    virtual ISL_StatusTypeDef AT_CMD_END(uint8_t* ptr, uint8_t* end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_SEP(uint8_t* ptr, uint8_t* end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_EQL(uint8_t* ptr, uint8_t* end, uint8_t nargs);
    virtual ISL_StatusTypeDef AT_CMD_START(uint8_t* ptr, uint8_t* end, uint8_t nargs);

    ISL_StatusTypeDef executeATCommand(uint8_t* command, uint8_t* rxbuff, uint16_t length, uint16_t timeout=100);

public:

    ATDevice(interfaces::UART uart, const uint16_t bsize = 128) : UARTDevice(uart), buff_size(bsize) {}

    ISL_StatusTypeDef writeAT(uint8_t* cmd, uint8_t* data, uint16_t timeout=100);
    ISL_StatusTypeDef readAT(uint8_t* cmd, uint8_t* rxbuff, uint16_t rxbuff_len = 128, uint16_t timeout=100);

};

}

#endif
