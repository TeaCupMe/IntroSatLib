#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"

#if defined(ARDUINO_AVR_ATmega328)
#include "Adapter/System.h"

namespace IntroSatLib
{
    ISL_StatusTypeDef ISLIRReceiver::put(uint8_t value)
    {
        uint16_t nextHead = (outputRawBuffHead + 1) % defaultOutputBSize;
        if (nextHead == outputRawBuffTail) 
        {
            // return ISL_ERROR;
            // outputRawBuff[outputRawBuffTail] = 0;
            outputRawBuffTail = (outputRawBuffTail + 1) % defaultOutputBSize;
        }
        outputRawBuff[outputRawBuffHead] = value;
        // Serial.println(outputRawBuff[outputRawBuffHead]);
        outputRawBuffHead = nextHead;
        return ISL_OK;
    }

    // ISL_StatusTypeDef ISLIRReceiver::putPackage(uint16_t* values, uint16_t plength)
    // {
    //     uint16_t nextHead = (outputRawBuffHead + plength);
    //     if (outputRawBuffHead < outputRawBuffTail && nextHead >= outputRawBuffTail)
    //     {
    //         outputRawBuffTail = (nextHead % defaultOutputBSize + 1);
    //     }
    //     nextHead = nextHead % defaultOutputBSize;
    //     if (outputRawBuffHead >= outputRawBuffTail && nextHead >= outputRawBuffTail && nextHead < outputRawBuffHead)
    //     {
    //         outputRawBuffTail = nextHead + 1;
    //     }
        
    //     for (uint16_t i = 0; i < plength; ++i)
    //     {
    //         outputRawBuff[(outputRawBuffHead + i) % defaultOutputBSize] = values[i];
    //     }
    //     outputRawBuffHead = nextHead;

    //     return ISL_OK;
    // }

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
                // putPackage(rawBuff, rawBuffN);

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
                // putPackage(rawBuff, rawBuffN);

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
        uint8_t oldSREG = SREG;
        cli();
        uint32_t currentTime = TimeSource();
        delta = (uint16_t)(currentTime - newT);

        if (delta < timings.errorScale) {
            SREG = oldSREG;
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

        SREG = oldSREG;
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


    // ISL_StatusTypeDef ISLIRReceiver::ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    // {
    //     for (uint16_t i = 0; (i < rawLength) && (i/16 < rxLength); i+=2)
    //     {
    //         if (abs((int32_t)rawData[i] - (int32_t)timings.mark0) <= timings.errorScale)
    //         {
    //             rxbuff[i/16] &= ~(0b1 << ((i%16)/2));
    //         } else if (abs((int32_t)rawData[i] - (int32_t)timings.mark1) <= timings.errorScale)
    //         {
    //             rxbuff[i/16] |= (0b1 << ((i%16)/2));
    //         } else
    //         {
    //             return ISL_ERROR; 
    //         }
    //     }
    //     return ISL_OK;
    // }

    // ISL_StatusTypeDef ISLIRReceiver::GetMessage(uint8_t* rxbuff, uint16_t rxLength)
    // {
    //     uint16_t buff[defaultOutputBSize];
    //     uint16_t n = GetData(buff, rxLength*16);
    //     RETURN_STATUS_IF_NOT_OK_SILENT(ISLDecode(buff, n, rxbuff, rxLength));
    //     return ISL_OK;
    // }

    uint16_t ISLIRReceiver::Available()
    {
        return (outputRawBuffHead >= outputRawBuffTail) ? 
                outputRawBuffHead - outputRawBuffTail : 
                outputRawBuffHead + (defaultOutputBSize - outputRawBuffTail);
    }

}

#endif
#endif