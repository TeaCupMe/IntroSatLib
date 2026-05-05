#define ISL_INTERNAL

#include "Device/IRTransceiver/IRTransceiver.h"

#if defined(ARDUINO)
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

    bool IRTransceiver::Available()
    {
        return receiver.Available();
    }
    
    ISL_StatusTypeDef IRTransceiver::GetRawData(uint16_t* buff, uint16_t length)
    {
        return receiver.GetRawData(buff, length);
    }

    ISL_StatusTypeDef IRTransceiver::ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    {
        return receiver.ISLDecode(rawData, rawLength, rxbuff, rxLength);
    }

    ISL_StatusTypeDef IRTransceiver::GetMessage(uint8_t* rxbuff, uint16_t rxLength)
    {
        return receiver.GetMessage(rxbuff, rxLength);
    }

}

#endif
