#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#if defined(ARDUINO_AVR_ATmega328)

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
        PreRecv = 2,
        RecvSpace = 3,
        RecvBit = 4
    };

    IRProtocol::ProtocolTimings timings = IRProtocol::ProtocolTimings();

    static constexpr uint16_t defaultBSize = 16;
    static constexpr uint16_t defaultOutputBSize = 64;

    State currentState = State::None;
    uint32_t newT = 0;
    uint16_t delta = 0;
    uint32_t (*TimeSource)() {system::GetTick};

    interfaces::GPIO receivePin;

    uint16_t rawBuffN = 0;
    uint16_t rawBuff[defaultBSize];

    volatile uint16_t outputRawBuffHead = 0;
    volatile uint16_t outputRawBuffTail = 0;
    volatile uint8_t outputRawBuff[defaultOutputBSize];
    volatile bool readingDataFlag = false;

    ISL_StatusTypeDef put(uint8_t value);
    ISL_StatusTypeDef pop(uint8_t *value);

    void IDLEStateProcess();
    void PreRecvStateProcess();
    void RecvSpaceStateProcess();
    void RecvBitStateProcess();

public:

    ISLIRReceiver(interfaces::GPIO _receivePin) : receivePin(_receivePin) { }

    ISL_StatusTypeDef Init()
    {
        currentState = State::Idle;
        return ISL_OK;
    }

    void ProcessReceiving();

    void SetTimeSource(uint32_t (*_TimeSource)()) { TimeSource = _TimeSource; }

    uint16_t Available();
    uint16_t GetData(uint8_t* buff, uint16_t length);

};

}

#endif
#endif