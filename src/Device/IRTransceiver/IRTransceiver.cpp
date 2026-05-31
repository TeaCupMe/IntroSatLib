#include "Device/IRTransceiver/IRTransceiver.h"

#if defined(ARDUINO_AVR_ATmega328)
#include "Adapter/System.h"

namespace IntroSatLib
{

ISL_StatusTypeDef IRTransceiver::Init()
{
    RETURN_STATUS_IF_NOT_OK_SILENT(receiver.Init());
    RETURN_STATUS_IF_NOT_OK_SILENT(transmitter.Init());
    return ISL_OK;
}

void IRTransceiver::ProcessReceiving()
{
    receiver.ProcessReceiving();
}

void IRTransceiver::SetTimeSource(uint32_t (*TimeSource)())
{
    receiver.SetTimeSource(TimeSource);
}

uint16_t IRTransceiver::Available()
{
    return receiver.Available();
}

uint16_t IRTransceiver::GetData(uint8_t* buff, uint16_t length)
{
    return receiver.GetData(buff, length);
}

ISL_StatusTypeDef IRTransceiver::Transmit(uint8_t* txBuff, uint16_t nbytes)
{
    return transmitter.Transmit(txBuff, nbytes);
}

void IRTransceiver::SetTone(void (*Tone)(interfaces::GPIO tonePin, uint16_t frequency))
{
    transmitter.SetTone(Tone);
}

void IRTransceiver::SetNoTone(void (*NoTone)(interfaces::GPIO tonePin))
{
    transmitter.SetNoTone(NoTone);
}

void IRTransceiver::SetDelaySource(void (*DelaySource)(uint32_t time))
{
    transmitter.SetDelaySource(DelaySource);
}

void IRTransceiver::SetModulationFrequence(uint16_t freq)
{
    transmitter.SetModulationFrequence(freq);
}

uint16_t IRTransceiver::GetModulationFrequence()
{
    return transmitter.GetModulationFrequence();
}

} // namespace IntroSatLib

#endif // ARDUINO_AVR_ATmega328