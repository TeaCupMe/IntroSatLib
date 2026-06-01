#include "Device/ISLIRTransceiver/ISLIRTransceiver.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

ISL_StatusTypeDef ISLIRTransceiver::Init()
{
    RETURN_STATUS_IF_NOT_OK_SILENT(receiver.Init());
    RETURN_STATUS_IF_NOT_OK_SILENT(transmitter.Init());
    return ISL_OK;
}

void ISLIRTransceiver::ProcessReceiving()
{
    receiver.ProcessReceiving();
}

void ISLIRTransceiver::SetTimeSource(uint32_t (*TimeSource)())
{
    receiver.SetTimeSource(TimeSource);
}

uint16_t ISLIRTransceiver::Available()
{
    return receiver.Available();
}

uint16_t ISLIRTransceiver::GetData(uint8_t* buff, uint16_t length)
{
    return receiver.GetData(buff, length);
}

ISL_StatusTypeDef ISLIRTransceiver::Transmit(uint8_t* txBuff, uint16_t nbytes)
{
    return transmitter.Transmit(txBuff, nbytes);
}

void ISLIRTransceiver::SetDelaySource(void (*DelaySource)(uint32_t time))
{
    transmitter.SetDelaySource(DelaySource);
}

void ISLIRTransceiver::SetModulationFrequence(uint16_t freq)
{
    transmitter.SetModulationFrequence(freq);
}

uint16_t ISLIRTransceiver::GetModulationFrequence()
{
    return transmitter.GetModulationFrequence();
}

} // namespace IntroSatLib