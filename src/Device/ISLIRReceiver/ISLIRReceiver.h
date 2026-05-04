#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#include "Device/Device.h"
#include "Adapter/GPIO.h"
// #include "Device/Base/BaseIRDecoder.h"
// #include "Device/ISLIRDecoder/ISLIRDecoder.h"


namespace IntroSatLib
{

class ISLIRReceiver : Device
{
public:

    struct ProtocolTimings
    {
        uint32_t markStart = 30;
        uint32_t mark0 = 10;
        uint32_t mark1 = 20;
        uint32_t space = 10;
        uint32_t errorScale = 5;
        uint32_t maxSpaceWidth = 25;
        uint32_t maxMarkWidth = 25;
    };

protected:

private:

    enum class State: uint8_t
    {
        None = 0,
        Idle = 1,
        RecvSpace = 2,
        RecvBit = 3
    };

    ProtocolTimings timings = ProtocolTimings();

    static constexpr uint16_t defaultTimeout = 1000;
    static constexpr uint16_t defaultBSize = 32;
    // uint16_t maxPulseWidth = 25;

    volatile State currentState = State::None;
    volatile uint8_t currentLogicLvl;
    volatile uint32_t oldT = 0, newT = 0;
    uint32_t (*TimeSource)() {system::GetTick};

    interfaces::GPIO receivePin;

    uint16_t rawBuffN = 0;
    uint16_t rawBuff[defaultBSize];

    bool available = false;
    uint16_t outputRawBuffN = 0;
    uint16_t outputRawBuff[defaultBSize];

    // inline bool DeltaT(uint32_t timing) { return abs(newT - oldT - timing) <= timings.errorScale; }

    inline bool Check(uint16_t data, uint16_t src) { return abs(data - src) <= timings.errorScale; }

    void IDLEStateProcess();
    void RecvSpaceStateProcess();
    void RecvBitStateProcess();

public:

    ISLIRReceiver(interfaces::GPIO _receivePin) : receivePin(_receivePin)
    {
        currentLogicLvl = receivePin.read();
    }

    ISL_StatusTypeDef Init()
    { 
        currentState = State::Idle;
        Serial.printf("My state: %d\n", (uint8_t)currentState);
        return ISL_OK;
    }

    ISL_StatusTypeDef ProcessReceiving();

    void SetTimeSource(uint32_t (*_TimeSource)()) { TimeSource = _TimeSource; }

    bool Available() { return available; }
    ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length);
    ISL_StatusTypeDef ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength);

};


}

#endif