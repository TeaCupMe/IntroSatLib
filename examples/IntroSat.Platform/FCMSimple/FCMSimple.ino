/* Подключение библиотеки IntroSatLib */
#include <IntroSatLib.h>
#include <ISL_Bootloader.h>

/* Подключаем файл для работы с аппаратным UART */
#include "HardwareSerial.h"
/* Подключаем файл для работы с аппаратным I2C */
#include "Wire.h"

/* Подключаем файлы для работы с устройствами */
#include "Device/LIS3MDL/LIS3MDL.h"
#include "Device/LSM6DS3/LSM6DS3.h"
#include "Device/MS5611/MS5611.h"
#include "Device/LM75A/LM75A.h"

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

/* Создаём объект для работы с аппаратным UART */
HardwareSerial usbSerial(PA3, PA2);

/* Создаём объект для работы с аппаратным I2C */
TwoWire Wire1(PB7, PB6);

LIS3MDL mag(Wire1, 0x1C);
LSM6DS3 ag(Wire1, 0x6A);
MS5611 bar(Wire1, 0x77);
LM75A temp(Wire1, 0x4A);

void setup() {
  usbSerial.begin(115200);

  Wire1.begin();
  delay(200);
  if (ag.Init() != ISL_OK)
  {
    usbSerial.println("AG init failed!");
  }
  if (mag.Init() != ISL_OK)
  {
    usbSerial.println("mag init failed!");
  }
  if (bar.Init() != ISL_OK)
  {
    usbSerial.println("bar init failed!");
  }
  if (temp.Init() != ISL_OK)
  {
    usbSerial.println("temp init failed!");
  }
}

void loop() {
  char command = 0;
  if (usbSerial.available())
  {
    command = usbSerial.read();
  }
  
  if (command == 'b') {
    EnterBootloader();
  } 
   else if (command == 'a')
  {
    Serial.println("AG: " + String(ag.AX()) + ", " + String(ag.AY()) + ", " + String(ag.AZ()));
  }
  else if (command == 'm')
  {
    Serial.println("MAG: " + String(mag.MX()) + ", " + String(mag.MY()) + ", " + String(mag.MZ()));
  }
  else if (command == 'g')
  {
    Serial.println("GYR: " + String(ag.GX()) + ", " + String(ag.GY()) + ", " + String(ag.GZ()));
  }
  else if (command == 'p')
  {
    Serial.println("BAR: " + String(bar.GetPressure()) + ", " + String(bar.GetTemperature()));
  }
  else if (command == 't')
  {
    Serial.println("TEMP: " + String(temp.GetTemperature()));
  }

  delay(100);
}