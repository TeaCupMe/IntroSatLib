#ifndef FSC_BT986_
#define FSC_BT986_

#include "Adapter/GPIO.h"
#include "Adapter/UART.h"
#include "Adapter/System.h"
#include "Device/ATDevice.h"


namespace IntroSatLib
{


class FSC_BT986 : public ATDevice
{
public:

    struct FSC_BT986Pins
    {
        interfaces::GPIO _reset = interfaces::GPIO();
        interfaces::GPIO _status = interfaces::GPIO();
        interfaces::GPIO _disconnect = interfaces::GPIO();
        interfaces::GPIO _mode = interfaces::GPIO();
    };

private:
    // interfaces::UART *_uart;
    FSC_BT986Pins _pins;

    // uint16_t executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length, uint16_t timeout=0xFFFF);

public:

    FSC_BT986(
        interfaces::UART uart,
        FSC_BT986Pins pins
    ): ATDevice(uart, 64), _pins(pins)
    { }

    ISL_StatusTypeDef Init() {

    }

    // ISL_StatusTypeDef writeAT(uint8_t* cmd, uint8_t* data, uint16_t timeout=0xFFFF);
    // ISL_StatusTypeDef readAT(uint8_t* cmd, uint8_t* rxbuffer, uint16_t rxbuffer_len = 1024, uint16_t timeout=0xFFFF);

    ISL_StatusTypeDef writePIN(uint8_t* pinCode, uint16_t timeout=0xFFFF);
    ISL_StatusTypeDef readPIN(uint8_t* rxbuff, uint16_t timeout=0xFFFF);

    ISL_StatusTypeDef readMAC(uint8_t* rxbuff, uint16_t timeout=0xFFFF);
    ISL_StatusTypeDef connect(uint8_t* mac, uint16_t timeout=0xFFFF);

    ISL_StatusTypeDef writePrgReset(uint16_t timeout=0xFFFF);
    ISL_StatusTypeDef writeHardReset(uint8_t state);

    ISL_StatusTypeDef writePrgMode(uint8_t state, uint16_t timeout=0xFFFF);
    ISL_StatusTypeDef writeHardMode(uint8_t state);

    uint8_t readStatus();

    ISL_StatusTypeDef writeHardDisconnect(uint8_t state);

};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */
