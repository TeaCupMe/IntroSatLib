#define ISL_INTERNAL

#include <stdlib.h>
#include "Device/ISLIRDecoder/ISLIRDecoder.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRDecoder::Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    {
        uint16_t itr = 0;
        while ((abs((int32_t)rawData[itr] - timings.markStart) >= timings.errorScale ||
               abs((int32_t)rawData[itr+1] - timings.spaceStart) >= timings.errorScale) &&
               itr < rawLength)
            itr += 2;
            
        for (uint16_t i = itr+2; (i-itr)/2 - 1 < rxLength && i < rawLength; i += 2)
        {
            if (abs((int32_t)rawData[i] - timings.mark1) < timings.errorScale && 
                abs((int32_t)rawData[i+1] - timings.space1) < timings.errorScale)
            {
                rxbuff[(i-itr)/2 - 1] = 1;
            }
            else if (abs((int32_t)rawData[i] - timings.mark0) < timings.errorScale && 
                     abs((int32_t)rawData[i+1] - timings.space0) < timings.errorScale)
            {
                rxbuff[(i-itr)/2 - 1] = 0;
            }
            else if (abs((int32_t)rawData[i] - timings.markEnd) < timings.errorScale)
            {
                break;
            }
            else return ISL_ERROR;
        }

        return ISL_OK;
    }

}