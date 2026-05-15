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

    static constexpr uint16_t defaultTimeout = 1000;
    static constexpr uint16_t defaultBSize = 16;
    static constexpr uint16_t defaultOutputBSize = 65;

    volatile State currentState = State::None;
    volatile uint8_t currentLogicLvl;
    volatile uint32_t oldT = 0, newT = 0;
    uint32_t (*TimeSource)() {system::GetTick};

    interfaces::GPIO receivePin;

    volatile uint16_t rawBuffN = 0;
    volatile uint16_t rawBuff[defaultBSize];

    volatile bool available = false;
    volatile uint16_t outputRawBuffHead = 0;
    volatile uint16_t outputRawBuffTail = 0;
    volatile uint16_t outputRawBuff[defaultBSize];
    volatile bool gettingDataFlag = false;

    volatile uint16_t readingStart = 0;
    volatile uint16_t readingEnd = 0;

    ISL_StatusTypeDef put(uint16_t value);
    ISL_StatusTypeDef putPackage(uint16_t* values, uint16_t plength);
    // ISL_StatusTypeDef get(uint16_t *value);
    ISL_StatusTypeDef pop(uint16_t *value);

    void IDLEStateProcess();
    void PreRecvStateProcess();
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
    uint16_t GetRawData(uint16_t* buff, uint16_t length);
    ISL_StatusTypeDef ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength);
    ISL_StatusTypeDef GetMessage(uint8_t* rxbuff, uint16_t rxLength);

};


}

#endif
#endif