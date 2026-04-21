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
        return decoder.Decode(rawData, rawLength, rxbuff, rxLength);
    }


    // ISL_StatusTypeDef ISLIRReceiver::Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint8_t* bytesResieved)
    // {
    //     uint16_t itr = 0;
    //     while ((abs((int32_t)rawData[itr] - timings.markStart) >= timings.errorScale ||
    //            abs((int32_t)rawData[itr+1] - timings.spaceStart) >= timings.errorScale) &&
    //            itr < rawLength)
    //         itr += 2;

    //     *bytesResieved = 0;
    //     for (uint16_t i = itr+2; (i-itr)/2 - 1 < rawLength; i += 2)
    //     {
    //         if (abs((int32_t)rawData[i] - timings.mark1) < timings.errorScale && 
    //             abs((int32_t)rawData[i+1] - timings.space1) < timings.errorScale)
    //         {
    //             Serial.printf("1: %d\n", (i-itr)/2 - 1);
    //             rxbuff[(i-itr)/2 - 1] = 1;
    //             *bytesResieved += 1;
    //         }
    //         else if (abs((int32_t)rawData[i] - timings.mark0) < timings.errorScale && 
    //                  abs((int32_t)rawData[i+1] - timings.space0) < timings.errorScale)
    //         {
    //             Serial.printf("0: %d\n", (i-itr)/2 - 1);
    //             rxbuff[(i-itr)/2 - 1] = 0;
    //             *bytesResieved += 1;
    //         }
    //         else if (abs((int32_t)rawData[i] - timings.markEnd) < timings.errorScale)
    //         {
    //             break;
    //         }
    //         else return ISL_ERROR;
    //     }

    //     return ISL_OK;
    // }


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