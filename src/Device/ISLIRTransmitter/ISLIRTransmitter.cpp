#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRTransmitter::Transmit(uint8_t* txBuff, uint16_t nbytes)
    {
        for (uint16_t itr = 0; itr < nbytes; ++itr)
        {
            RETURN_STATUS_IF_NOT_OK_SILENT(SendByte(txBuff[itr]));
        }

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


    ISL_StatusTypeDef ISLIRTransmitter::SendByte(uint8_t byte)
    {
        uint16_t buff[16];
        uint16_t n = GenerateRawTxData(&byte, 8, buff, 16);
        if (n != 16) return ISL_ERROR;

        txPin.tone(38000);
        DelaySource(timings.markStart);
        txPin.noTone();
        DelaySource(timings.spaceStart);
        for (uint16_t i = 0; i < n; i+=2)
        {
            txPin.tone(38000);
            DelaySource(buff[i]);
            txPin.noTone();
            DelaySource(buff[i+1]);
        }
        txPin.tone(38000);
        DelaySource(timings.maxMarkWidth);
        txPin.noTone();
        DelaySource(timings.maxSpaceWidth);

        return ISL_OK;
    }

}

#endif