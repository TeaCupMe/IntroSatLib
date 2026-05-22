#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#if defined(ARDUINO_AVR_ATmega328)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"

namespace IntroSatLib
{

class IRTransceiver : Device
{
private:
    ISLIRReceiver receiver;
    ISLIRTransmitter transmitter;

public:
    IRTransceiver(interfaces::GPIO rxPin, interfaces::GPIO txPin, uint16_t modulationFreq = 38000)
        : receiver(rxPin)
        , transmitter(txPin, modulationFreq)
    { };

    ISL_StatusTypeDef Init();

    // ----- Receiver interface -----
    void ProcessReceiving();
    void SetTimeSource(uint32_t (*TimeSource)());
    uint16_t Available();
    uint16_t GetData(uint8_t* buff, uint16_t length);

    // ----- Transmitter interface -----
    ISL_StatusTypeDef Transmit(uint8_t* txBuff, uint16_t nbytes);
    void SetTone(void (*Tone)(interfaces::GPIO tonePin, uint16_t frequency));
    void SetNoTone(void (*NoTone)(interfaces::GPIO tonePin));
    void SetDelaySource(void (*DelaySource)(uint32_t time));
    void SetModulationFrequence(uint16_t freq);
    uint16_t GetModulationFrequence();
};

} // namespace IntroSatLib

#endif // ARDUINO_AVR_ATmega328
#endif // IRTRANSCEIVER_H_