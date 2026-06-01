#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Adapter/System.h"

namespace IntroSatLib
{
    ISL_StatusTypeDef ISLIRReceiver::put(uint8_t value)
    {
        uint16_t nextHead = (outputRawBuffHead + 1) % defaultOutputBSize;
        if (nextHead == outputRawBuffTail) 
        {
            outputRawBuffTail = (outputRawBuffTail + 1) % defaultOutputBSize;
        }
        outputRawBuff[outputRawBuffHead] = value;
        outputRawBuffHead = nextHead;
        return ISL_OK;
    }

    ISL_StatusTypeDef ISLIRReceiver::pop(uint8_t *value)
    {
        if (outputRawBuffHead == outputRawBuffTail)
        {
            return ISL_ERROR;
        }
        *value = outputRawBuff[outputRawBuffTail];
        outputRawBuff[outputRawBuffTail] = 0;
        outputRawBuffTail = (outputRawBuffTail + 1) % defaultOutputBSize;
        return ISL_OK;
    }


    void ISLIRReceiver::IDLEStateProcess()
    {
        if (receivePin.read() == 1)
        {
            if (abs(delta - timings.markStart) <= timings.errorScale)
            {
                currentState = State::PreRecv;
            }
        }
        
        return;
    }

    void ISLIRReceiver::PreRecvStateProcess()
    {
        if (abs(delta - timings.spaceStart) <= timings.errorScale)
        {
            rawBuffN = 0;
            currentState = State::RecvBit;
            return;
        }

        currentState = State::Idle;
        return;
    }

    void ISLIRReceiver::RecvSpaceStateProcess()
    {
        if (delta > timings.maxSpaceWidth - timings.errorScale)
        {
            currentState = State::Idle;

            if (!readingDataFlag)
            {
                uint8_t data = 0;
                for (uint8_t i = 0; (i < rawBuffN); i+=2)
                {
                    if (abs((int32_t)rawBuff[i] - (int32_t)timings.mark0) <= timings.errorScale)
                    {
                        data &= ~(0b1 << ((i%16)/2));
                    } else if (abs((int32_t)rawBuff[i] - (int32_t)timings.mark1) <= timings.errorScale)
                    {
                        data |= (0b1 << ((i%16)/2));
                    } else
                    {
                        return;
                    }
                }

                put(data);
            }
            return;
        } else
        {
            if (rawBuffN < defaultBSize)
            {
                rawBuff[rawBuffN] = delta;
                rawBuffN++;
            }
            currentState = State::RecvBit;
        }

        return;
    }

    void ISLIRReceiver::RecvBitStateProcess()
    {
        if (delta > timings.maxMarkWidth - timings.errorScale)
        {
            currentState = State::Idle;

            if (!readingDataFlag)
            {
                uint8_t data = 0;
                for (uint8_t i = 0; (i < rawBuffN); i+=2)
                {
                    if (abs((int32_t)rawBuff[i] - (int32_t)timings.mark0) <= timings.errorScale)
                    {
                        data &= ~(0b1 << ((i%16)/2));
                    } else if (abs((int32_t)rawBuff[i] - (int32_t)timings.mark1) <= timings.errorScale)
                    {
                        data |= (0b1 << ((i%16)/2));
                    } else
                    {
                        return;
                    }
                }

                put(data);
            }
            return;
        } else
        {
            if (rawBuffN < defaultBSize)
            {
                rawBuff[rawBuffN] = delta;
                rawBuffN++;
            }
            currentState = State::RecvSpace;
        }

        return;
    }

    void ISLIRReceiver::ProcessReceiving()
    {
        uint32_t currentTime = TimeSource();
        delta = (uint16_t)(currentTime - newT);

        if (delta < timings.errorScale) {
            return;
        }

        newT = currentTime;
        
        switch (currentState)
        {
            case State::Idle:
                IDLEStateProcess();
                break;

            case State::PreRecv:
                PreRecvStateProcess();
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

        return;
    }


    uint16_t ISLIRReceiver::GetData(uint8_t* buff, uint16_t length)
    {
        readingDataFlag = true;

        if (!Available()) return ISL_ERROR;

        uint16_t itr;
        for (itr = 0; (itr < length); ++itr)
        {
            if (pop(buff + itr) != ISL_OK) break;
        }
        readingDataFlag = false;
        return itr;
    }

    uint16_t ISLIRReceiver::Available()
    {
        return (outputRawBuffHead >= outputRawBuffTail) ? 
                outputRawBuffHead - outputRawBuffTail : 
                outputRawBuffHead + (defaultOutputBSize - outputRawBuffTail);
    }

}

#endif