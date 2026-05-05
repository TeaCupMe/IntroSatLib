#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"

#if defined(ARDUINO)
#include "Adapter/System.h"

namespace IntroSatLib
{


    void ISLIRReceiver::IDLEStateProcess()
    {
        if (receivePin.read() == 0)
        {
            uint16_t delta = (uint16_t)(newT-oldT);
            if (abs(delta - timings.markStart) <= timings.errorScale)
            rawBuffN = 0;
            currentState = State::RecvBit;
        }
        
        return;
    }

    void ISLIRReceiver::RecvSpaceStateProcess()
    {

        uint16_t delta = (uint16_t)(newT-oldT);
        if (delta > timings.maxSpaceWidth - timings.errorScale)
        {
            available = true;
            outputRawBuffN = rawBuffN;
            currentState = State::Idle;

            for (uint16_t i = 0; i < rawBuffN; i++)
            {
                outputRawBuff[i] = rawBuff[i];
            }

            return;
        } else
        {
            rawBuff[rawBuffN] = delta;
            rawBuffN++;
            currentState = State::RecvBit;
        }

        return;
    }

    void ISLIRReceiver::RecvBitStateProcess()
    {
        uint16_t delta = (uint16_t)(newT-oldT);
        if (delta > timings.maxMarkWidth - timings.errorScale)
        {
            available = true;
            outputRawBuffN = rawBuffN;
            currentState = State::Idle;

            for (uint16_t i = 0; i < rawBuffN; i++)
            {
                outputRawBuff[i] = rawBuff[i];
            }

            return;
        } else
        {
            rawBuff[rawBuffN] = delta;
            rawBuffN++;
            currentState = State::RecvSpace;
        }

        return;
    }

    void ISLIRReceiver::ProcessReceiving()
    {
        uint8_t oldSREG = SREG;
        cli();

        oldT = newT;
        newT = TimeSource();

        switch (currentState)
        {
            case State::Idle:
                IDLEStateProcess();
                break;
            
            case State::RecvSpace:
                RecvSpaceStateProcess();
                break;

            case State::RecvBit:
                RecvBitStateProcess();
                break;

            default:
                break;
        }

        SREG = oldSREG;
        return;
    }


    ISL_StatusTypeDef ISLIRReceiver::GetRawData(uint16_t* buff, uint16_t length)
    {
        if (!Available()) return ISL_ERROR;
        available = false;

        for (uint16_t i = 0; (i < length) && (i < outputRawBuffN); i++)
        {
            buff[i] = outputRawBuff[i];
        }

        return ISL_OK;
    }


    ISL_StatusTypeDef ISLIRReceiver::ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    {
        for (uint16_t i = 0; (i < rawLength) && (i/16 < rxLength); i+=2)
        {
            if (abs((int32_t)rawData[i] - (int32_t)timings.mark0) <= timings.errorScale)
            {
                rxbuff[i/16] &= ~(0b1 << (i/2));
            } else if (abs((int32_t)rawData[i] - (int32_t)timings.mark1) <= timings.errorScale)
            {
                rxbuff[i/16] |= (0b1 << (i/2));
            } else {
                return ISL_ERROR; 
            }
        }
        return ISL_OK;
    }

    ISL_StatusTypeDef ISLIRReceiver::GetMessage(uint8_t* rxbuff, uint16_t rxLength)
    {
        uint16_t buff[defaultBSize];
        RETURN_STATUS_IF_NOT_OK_SILENT(GetRawData(buff, defaultBSize));
        RETURN_STATUS_IF_NOT_OK_SILENT(ISLDecode(buff, defaultBSize, rxbuff, rxLength));
        return ISL_OK;
    }

}

#endif
#endif