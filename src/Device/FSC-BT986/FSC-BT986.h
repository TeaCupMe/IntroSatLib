#ifndef FSC_BT986_
#define FSC_BT986_

#include "Adapter/GPIO.h"
#include "Adapter/UART.h"
#include "Adapter/System.h"
#include "Commands.h"
#include <string.h>


namespace IntroSatLib
{


class FSC_BT986
{
public:

    struct FSC_BT986Pins
    {
        interfaces::GPIO _reset;
        interfaces::GPIO _status;
        interfaces::GPIO _disconnect;
        interfaces::GPIO _mode;
    };

private:
    interfaces::UART *_uart;
    FSC_BT986Pins _pins;

    uint32_t timer;
    uint16_t timeout = 1000;

public:

    FSC_BT986(
        interfaces::UART *uart,
        FSC_BT986Pins pins
    ): _uart(uart), _pins(pins)
    { }

    ISL_StatusTypeDef Init() {

    }

    uint16_t getTimeout() {return timeout;}
    void setTimeout(uint16_t newTimeout) {timeout = newTimeout;}

    uint16_t executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length);

};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */