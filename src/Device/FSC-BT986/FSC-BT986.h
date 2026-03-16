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
        interfaces::GPIO reset = interfaces::GPIO();
        interfaces::GPIO status = interfaces::GPIO();
        interfaces::GPIO disconnect = interfaces::GPIO();
        interfaces::GPIO mode = interfaces::GPIO();
    };

private:
    FSC_BT986Pins pins;

public:

    FSC_BT986(
        interfaces::UART uart,
        FSC_BT986Pins _pins,
        const uint16_t bsize
    ): ATDevice(uart, bsize), pins(_pins) { }

    ISL_StatusTypeDef Init();

    ISL_StatusTypeDef writePIN(uint8_t* pinCode, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef readPIN(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef readMAC(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef connect(uint8_t* mac, uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef writePrgReboot(uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef writeHardReboot(uint8_t state);

    ISL_StatusTypeDef writePrgMode(uint8_t state, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef writeHardMode(uint8_t state);

    uint8_t readStatus();

    ISL_StatusTypeDef writeHardDisconnect(uint8_t state);

};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */
