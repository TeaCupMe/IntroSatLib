#define ISL_INTERNAL

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


    ISL_StatusTypeDef IRTransceiver::TransmitIR(uint8_t* txBuff, uint16_t length)
    {
        return transmitter.Transmit(txBuff, length);
    }


    void IRTransceiver::ProcessReceivingIR()
    {
        return receiver.ProcessReceiving();
    }

    uint16_t IRTransceiver::Available()
    {
        return receiver.Available();
    }
    
    ISL_StatusTypeDef IRTransceiver::GetData(uint8_t* buff, uint16_t length)
    {
        return receiver.GetData(buff, length);
    }

}

#endif
