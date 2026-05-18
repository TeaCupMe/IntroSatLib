#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"

#if defined(ARDUINO_AVR_ATmega328)
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef ISLIRReceiver::put(uint16_t value)
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

    ISL_StatusTypeDef ISLIRReceiver::pop(uint16_t *value)
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
            // uint16_t delta = (uint16_t)(newT-oldT);
            if (abs(delta - timings.markStart) <= timings.errorScale)
            {
                // Serial.println("Go to PreRecv");
                // rawBuffN = 0;
                currentState = State::PreRecv;
            }
        }
        
        return;
    }

    void ISLIRReceiver::PreRecvStateProcess()
    {
        // uint16_t delta = (uint16_t)(newT-oldT);
        if (abs(delta - timings.spaceStart) <= timings.errorScale)
        {
            // Serial.println("Go to RecvBit");
            rawBuffN = 0;
            currentState = State::RecvBit;
            return;
        }

        currentState = State::Idle;
        return;
    }

    void ISLIRReceiver::RecvSpaceStateProcess()
    {
        // uint16_t delta = (uint16_t)(newT-oldT);
        if (delta > timings.maxSpaceWidth - timings.errorScale)
        {
            // available = true;
            currentState = State::Idle;

            if (!gettingDataFlag)
            {
                available = true;
                for (uint16_t i = 0; i < rawBuffN; i++)
                {
                    put(rawBuff[i]);
                }
            }

            // Serial.println("Space");
            // for (uint16_t i = 0; i < defaultBSize; ++i) 
            // {
            //     Serial.print(rawBuff[i]);
            //     Serial.print("\t");
            //     if (i%8 == 7) Serial.println();
            // }

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
        // uint16_t delta = (uint16_t)(newT-oldT);
        if (delta > timings.maxMarkWidth - timings.errorScale)
        {
            // available = true;
            currentState = State::Idle;

            if (!gettingDataFlag)
            {
                available = true;
                for (uint16_t i = 0; i < rawBuffN; i++)
                {
                    put(rawBuff[i]);
                    // Serial.println(rawBuff[i]);
                }
            }

            // Serial.println("Buff after saving:");
            // for (uint16_t i = 0; i < defaultOutputBSize; ++i)
            // {
            //     Serial.print(outputRawBuff[i]);
            //     Serial.print("\t");
            //     Serial.flush();
            //     if (i%8 == 7) Serial.println();
            // }
            // Serial.println();

            // Serial.println("Bit");
            // for (uint16_t i = 0; i < defaultBSize; ++i) 
            // {
            //     Serial.print(rawBuff[i]);
            //     Serial.print("\t");
            //     if (i%8 == 7) Serial.println();
            // }

            return;
        } else
        {
            if (rawBuffN < defaultBSize)
            {
                rawBuff[rawBuffN] = delta;
                rawBuffN++;
                // Serial.println(delta);
            }
            currentState = State::RecvSpace;
        }

        return;
    }

    void ISLIRReceiver::ProcessReceiving()
    {
        uint8_t oldSREG = SREG;
        cli();

        // oldT = newT;
        // newT = TimeSource();

        // Снять текущее время, не трогая старые метки
        uint32_t currentTime = TimeSource();
        delta = (uint16_t)(currentTime - newT); // newT хранит время последнего *принятого* перепада

        // Порог фильтрации: всё, что короче errorScale (или, например, 5), считаем помехой
        if (delta < timings.errorScale) {
            // Помеха – игнорируем, ничего не обновляем
            // Serial.println(delta);
            SREG = oldSREG;
            return;
        }

        // Достоверный перепад: обновляем временные метки
        oldT = newT;
        newT = currentTime;
        
        // Serial.println(delta);
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


    uint16_t ISLIRReceiver::GetRawData(uint16_t* buff, uint16_t length)
    {
        // uint8_t oldSREG = SREG;
        // cli();

        gettingDataFlag = true;

        if (!Available()) return ISL_ERROR;

        // Serial.println("Buff prefer:");
        // for (uint16_t i = 0; i < defaultOutputBSize; ++i)
        // {
        //     Serial.print(outputRawBuff[i]);
        //     Serial.print("\t");
        //     Serial.flush();
        //     if (i%8 == 7) Serial.println();
        // }
        // Serial.println();

        uint16_t itr;
        for (itr = 0; (itr < length); ++itr)
        {
            // Serial.print(itr);
            // Serial.print("\t");
            // Serial.print(*(buff + itr));
            if (pop(buff + itr) != ISL_OK) break;
            // Serial.print("\t");
            // Serial.println(*(buff + itr));
        }

        // Serial.println("Buff after:");
        // for (uint16_t i = 0; i < defaultOutputBSize; ++i)
        // {
        //     Serial.print(outputRawBuff[i]);
        //     Serial.print("\t");
        //     Serial.flush();
        //     if (i%8 == 7) Serial.println();
        // }
        // Serial.println();


        if (outputRawBuffHead == outputRawBuffTail) available = false;

        gettingDataFlag = false;
        // SREG = oldSREG;
        return itr;
    }


    ISL_StatusTypeDef ISLIRReceiver::ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength)
    {
        Serial.println("Decoding");
        for (uint16_t i = 0; (i < rawLength) && (i/16 < rxLength); i+=2)
        {
            // Serial.print(i);
            // Serial.print("\t");
            // Serial.print(rawData[i]);
            // Serial.print("\t");
            if (abs((int32_t)rawData[i] - (int32_t)timings.mark0) <= timings.errorScale)
            {
                rxbuff[i/16] &= ~(0b1 << ((i%16)/2));
                // Serial.print("0 to ");
                // Serial.println(i/16);
            } else if (abs((int32_t)rawData[i] - (int32_t)timings.mark1) <= timings.errorScale)
            {
                rxbuff[i/16] |= (0b1 << ((i%16)/2));
                // Serial.print("1 to ");
                // Serial.println(i/16);
            } else {
                return ISL_ERROR; 
            }
        }
        return ISL_OK;
    }

    ISL_StatusTypeDef ISLIRReceiver::GetMessage(uint8_t* rxbuff, uint16_t rxLength)
    {
        uint16_t buff[defaultOutputBSize];
        uint16_t n = GetRawData(buff, defaultBSize);
        RETURN_STATUS_IF_NOT_OK_SILENT(ISLDecode(buff, n, rxbuff, rxLength));
        return ISL_OK;
    }

}

#endif
#endif