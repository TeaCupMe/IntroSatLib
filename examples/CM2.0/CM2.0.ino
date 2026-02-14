#include "IntroSatLib_def.h"
#include "Modules/Camera.h"
#include "HardwareSerial.h"
#include "Bootloader.h"

HardwareSerial cameraSerial(USART3);

using namespace IntroSatLib;

Camera cam(Serial1);
Chunk currentChunk;
ImageProperties prop;

void setup()
{
    Serial1.begin(230400);
    Serial.begin(115200, SERIAL_8E1);
    cam.Init(400, 400);
}

void loop()
{
    if (Serial.available())
    {
        char s = Serial.read();
        if (s == 'b')
        {
            EnterBootloader();
        }
        else if (s == 't')
        {
            if (cam.TakePicture())
            {
                Serial.println("Ошибка!");
                return;
            }
            Serial.println("Снимок сделан!");
            cam.GetImageProperties();
            //   prop = cam.prop;
            //   Serial.print("")
        }
        else if (s == 'i')
        {
            cam.GetImageProperties();
            prop = cam.prop;

            if (prop.numberOfChunks == 0)
            {
                Serial.println("Снимок не сделан!");
                return;
            }
            Serial.println("Данные снимка ->");
            for (int i = 0; i < prop.numberOfChunks; i++)
            {
                cam.GetNextChunk();
                currentChunk = cam.chunk;
                Serial.write((uint8_t *)&currentChunk.payload, currentChunk.payloadLength);
            }
            Serial.println("<-Конец данных снимка.");
        }
    }
}
