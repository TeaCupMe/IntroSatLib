#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRReceiver::GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout)
    {
        bool endReceiving = false;
        uint32_t startTime = system::GetTick();
        while(receivePin.read() != 0)
		{
			if ((system::GetTick() - startTime) > timeout) { return ISL_TIMEOUT; }
		}
        uint32_t endTime = system::GetTick();

        for (uint16_t i = 0; i < length; i++)
        {
            startTime = endTime;
            while(receivePin.read() != (i+1)%2)
            {
                if ((system::GetTick() - startTime) > maxPulseWidth) endReceiving = true;
            }
            endTime = system::GetTick();

            if (endReceiving) break;
            buff[i] = endTime - startTime;
        }

        return ISL_OK;
    }

    ISL_StatusTypeDef ISLIRReceiver::Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    {
        return decoder->Decode(rawData, rawLength, rxbuff, rxLength);
    }


    ISL_StatusTypeDef ISLIRReceiver::ReceiveIR(uint8_t* buff, uint8_t length, uint16_t timeout)
    {
        uint16_t rawBuff[128];
        RETURN_STATUS_IF_NOT_OK_SILENT(GetRawData(rawBuff, 128, timeout));

        uint8_t nbytes;
        RETURN_STATUS_IF_NOT_OK_SILENT(Decode(rawBuff, 128, buff, length));
        return ISL_OK;
    }



}

#endif