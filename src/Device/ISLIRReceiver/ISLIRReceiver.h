#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#if defined(ARDUINO)

#include "Device/Device.h"
#include "Adapter/GPIO.h"
#include "Device/ISLIRProtocol/ISLIRProtocol.h"


namespace IntroSatLib
{

class ISLIRReceiver : Device
{
public:

protected:

private:

    enum class State: uint8_t
    {
        None = 0,
        Idle = 1,
        RecvSpace = 2,
        RecvBit = 3
    };

    IRProtocol::ProtocolTimings timings = IRProtocol::ProtocolTimings();

    static constexpr uint16_t defaultTimeout = 1000;
    static constexpr uint16_t defaultBSize = 16;

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
        return ISL_OK;
    }

    void ProcessReceiving();

    void SetTimeSource(uint32_t (*_TimeSource)()) { TimeSource = _TimeSource; }

    bool Available() { return available; }
    ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length);
    ISL_StatusTypeDef ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength);
    ISL_StatusTypeDef GetMessage(uint8_t* rxbuff, uint16_t rxLength);

};


}

#endif
#endif