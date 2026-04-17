#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#include "Device/Device.h"
#include "Adapter/GPIO.h"


namespace IntroSatLib
{

class ISLIRReceiver : Device
{
public:

    struct ProtocolTimings {
        uint16_t markStart = 30;
        uint16_t spaceStart = 10;
        uint16_t mark0 = 10;
        uint16_t space0 = 10;
        uint16_t mark1 = 20;
        uint16_t space1 = 10;
        uint16_t errorScale = 5;
    };

protected:

private:

    static constexpr uint16_t defaultTimeout = 1000;

    interfaces::GPIO receivePin;
    ProtocolTimings timings;

    // ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout=defaultTimeout);

    // ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff);

public:

    ISLIRReceiver(interfaces::GPIO _receivePin, ProtocolTimings _timings ): receivePin(_receivePin),
                                                                            timings(_timings) { }

    ISL_StatusTypeDef Init() { }

    ISL_StatusTypeDef ReceiveIR(uint8_t* buff, uint8_t length, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint8_t* bytesResieved);

};


}

#endif