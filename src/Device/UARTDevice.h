#ifndef UARTDEVICE_H_
#define UARTDEVICE_H_

#include "Adapter/UART.h"
#include "Device.h"

namespace IntroSatLib {

class UARTDevice : public Device {
private:
    interfaces::UART _uart;

protected:

public:

    UARTDevice(interfaces::UART uart) : _uart(uart) {}
    UARTDevice(const UARTDevice& other) : _uart(other._uart) {}
	UARTDevice(UARTDevice&& other) : _uart(other._uart) {}

    ISL_StatusTypeDef WriteUART(uint8_t* buf, uint8_t nBytes, uint16_t timeout = 0xFFFF);
	ISL_StatusTypeDef ReadUART(uint8_t* buf, uint8_t nBytes = 1, uint16_t timeout = 0xFFFF);

    uint8_t available();

};

}

#endif
