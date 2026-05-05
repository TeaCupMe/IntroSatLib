#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRTransmitter::Transmit(uint8_t* txBuff, uint16_t nBits)
    {
        if (nBits/8 > defaultBSize) return ISL_ERROR;

        uint16_t buff[defaultBSize];
        uint16_t n = GenerateRawTxData(txBuff, nBits, buff, defaultBSize);

        Tone(txPin, 38000);
        DelaySource(timings.markStart);
        NoTone(txPin);
        DelaySource(timings.spaceStart);
        for (uint16_t i = 0; i < n; i+=2)
        {
            Tone(txPin, 38000);
            DelaySource(buff[i]);
            NoTone(txPin);
            DelaySource(buff[i+1]);
        }
        Tone(txPin, 38000);
        DelaySource(timings.maxSpaceWidth);
        NoTone(txPin);

        return ISL_OK;
    }


    uint16_t ISLIRTransmitter::GenerateRawTxData(uint8_t* txBuff, uint16_t nBits, uint16_t* buff, uint16_t length)
    {
        for (uint16_t i = 0; (i < nBits) && (i/8 < length); i++)
        {
            if ((txBuff[i/8] >> (i%8)) & 0b1)
            {
                buff[2*i] = timings.mark1;
                buff[2*i+1] = timings.space1;
            }
            else
            {
                buff[2*i] = timings.mark0;
                buff[2*i+1] = timings.space0;
            }
        }

        return nBits*2 > length ? length : nBits*2;
    }



}

#endif