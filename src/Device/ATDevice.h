#ifndef ATDEVICE_H_
#define ATDEVICE_H_

#include "Device/UARTDevice.h"

#define AT_OK       "\r\nOK\r\n"
#define AT_ERROR    "\r\nERROR\r\n"


namespace IntroSatLib {

class ATDevice: public UARTDevice {
private:

public:

    const uint16_t buff_size;

    template<typename... Args>
    ISL_StatusTypeDef AT_CMD(uint8_t* buffer, size_t buffer_size, const char* cmd, Args... args);

    uint16_t executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length, uint16_t timeout=0xFFFF);

public:

    ATDevice(interfaces::UART uart, const uint16_t bsize = 128) : UARTDevice(uart), buff_size(bsize) {}

    ISL_StatusTypeDef writeAT(uint8_t* cmd, uint8_t* data, uint16_t timeout=0xFFFF);
    ISL_StatusTypeDef readAT(uint8_t* cmd, uint8_t* rxbuffer, uint16_t rxbuffer_len = 1024, uint16_t timeout=0xFFFF);

};

}

#endif
