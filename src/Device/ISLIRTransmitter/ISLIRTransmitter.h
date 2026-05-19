#ifndef ISLIRTRANSMITTER_H_
#define ISLIRTRANSMITTER_H_

#if defined(ARDUINO_AVR_ATmega328)

#include "Device/Device.h"
#include "Adapter/GPIO.h"
#include "Device/ISLIRProtocol/ISLIRProtocol.h"


namespace IntroSatLib
{

class ISLIRTransmitter : Device
{

public:

protected:

private:

    interfaces::GPIO txPin;

    IRProtocol::ProtocolTimings timings = IRProtocol::ProtocolTimings();
    uint16_t modulationFrequence;

    void (*Tone)(interfaces::GPIO tonePin, uint16_t frequency) { tone };
    void (*NoTone)(interfaces::GPIO tonePin) { noTone };
    void (*DelaySource)(uint32_t time) {system::Delay};

    uint16_t GenerateRawTxData(uint8_t* txBuff, uint16_t nBits, uint16_t* buff, uint16_t length);
    ISL_StatusTypeDef SendByte(uint8_t byte);

public:

    ISLIRTransmitter(interfaces::GPIO _txPin, uint16_t frequence = 38000) : txPin(_txPin), 
                                                                            modulationFrequence(frequence) { }
    ISL_StatusTypeDef Init() { return ISL_OK; }

    ISL_StatusTypeDef Transmit(uint8_t* txBuff, uint16_t nbytes);

    void SetTone(void (*_Tone)(interfaces::GPIO tonePin, uint16_t frequency)) { Tone = _Tone; }
    void SetNoTone(void (*_NoTone)(interfaces::GPIO tonePin)) { NoTone = _NoTone; }
    void SetDelaySource(void (*_DelaySource)(uint32_t time)) { DelaySource = _DelaySource; }

    void SetModulationFrequence(uint16_t _frequence) { modulationFrequence = _frequence; }
    uint16_t GetModulationFrequence() { return modulationFrequence; }

};

}

#endif
#endif