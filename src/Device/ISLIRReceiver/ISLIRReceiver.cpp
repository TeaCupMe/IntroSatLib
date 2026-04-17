#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRReceiver::GetRawData(uint16_t* buff, uint16_t length, uint16_t timeout)
    {
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
                if ((system::GetTick() - startTime) > timeout) { return ISL_TIMEOUT; }
            }
            endTime = system::GetTick();

            buff[i] = endTime - startTime;
        }

        return ISL_OK;
    }

    ISL_StatusTypeDef ISLIRReceiver::Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint8_t* bytesResieved)
    {
        uint16_t itr = 0;
        while (abs(rawData[itr] - timings.markStart) >= timings.errorScale && itr < rawLength) itr++;
        if (itr >= rawLength) return ISL_ERROR;

        *bytesResieved = 0;
        for (uint16_t i = itr+2; i < rawLength; i += 2)
        {            
            if (abs(rawData[i] - timings.mark1) < timings.errorScale)
            {
                rxbuff[i/2] = 1; 
                *bytesResieved += 1;
            }
            else if (abs(rawData[i] - timings.mark0) < timings.errorScale)
            {
                rxbuff[i/2] = 0;
                *bytesResieved += 1;
            }
            else return ISL_ERROR;
        }

        return ISL_OK;
    }


    ISL_StatusTypeDef ISLIRReceiver::ReceiveIR(uint8_t* buff, uint8_t length, uint16_t timeout)
    {
        uint16_t rawBuff[128];
        RETURN_STATUS_IF_NOT_OK_SILENT(GetRawData(rawBuff, 128, timeout));

        uint8_t nbytes;
        RETURN_STATUS_IF_NOT_OK_SILENT(Decode(rawBuff, (uint16_t)length*2, buff, &nbytes));
        return ISL_OK;
    }



}

#endif