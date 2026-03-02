#ifndef FSC_BT986_
#define FSC_BT986_

#include "Adapter/GPIO.h"
#include "Adapter/UART.h"
#include "Adapter/System.h"


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

    // uint32_t timer;
    // uint16_t timeout = 100;

public:

    FSC_BT986(
        interfaces::UART *uart,
        FSC_BT986Pins pins
    ): _uart(uart), _pins(pins)
    { }

    ISL_StatusTypeDef Init() {

    }

    uint16_t executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length, uint16_t timeout = 0xFFFF);

    ISL_StatusTypeDef writePIN(uint8_t* pinCode, uint16_t timeout = 0xFFFF);
    ISL_StatusTypeDef readPIN(uint8_t* rxbuffer, uint16_t timeout = 0xFFFF);

    ISL_StatusTypeDef readMAC(uint8_t* rxbuffer, uint16_t timeout = 0xFFFF);
    ISL_StatusTypeDef connect(uint8_t* mac, uint16_t timeout = 0xFFFF);

    ISL_StatusTypeDef writePrgReset(uint16_t timeout = 0xFFFF);
    ISL_StatusTypeDef writeHardReset(uint8_t state);

    ISL_StatusTypeDef writeCommandMode();

    ISL_StatusTypeDef writePrgTpMode(uint8_t state, uint16_t timeout = 0xFFFF);
    ISL_StatusTypeDef writeHardTpMode(uint8_t state);

    ISL_StatusTypeDef readStatus(uint8_t* rxbuffer);

    ISL_StatusTypeDef writeHardDisconnect(uint8_t state);

};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */