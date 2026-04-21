#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#include "Device/Device.h"
#include "Adapter/GPIO.h"
#include "Device/Base/BaseIRDecoder.h"
#include "Device/ISLIRDecoder/ISLIRDecoder.h"


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
        uint16_t markEnd = 30;
        uint16_t spaceEnd = 10;
        uint16_t errorScale = 5;
    };

protected:

private:

    static constexpr uint16_t defaultTimeout = 1000;
    uint16_t maxPulseWidth = 100;

    interfaces::GPIO receivePin;
    BaseIRDecoder decoder;
    // ProtocolTimings timings;

    // ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout=defaultTimeout);

    // ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff);

public:

    ISLIRReceiver(interfaces::GPIO _receivePin, BaseIRDecoder _decoder) : receivePin(_receivePin),
                                                                            decoder(_decoder) { }

    ISL_StatusTypeDef Init() { }

    ISL_StatusTypeDef ReceiveIR(uint8_t* buff, uint8_t length, uint16_t timeout=defaultTimeout);

    uint16_t GetmaxPulseWidth() { return maxPulseWidth; }
    void setMaxPulseWidth(uint16_t newMaxPulseWidth) { maxPulseWidth = newMaxPulseWidth; }

    ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength);

};


}

#endif