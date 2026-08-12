// Подключение библиотек
#include "IntroSatLib.h"
#include "Device/CC1101/CC1101.h"
#include <SPI.h>

// Объявляем пин CS для CC1101
#define CC1101_CS_PIN 4

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

// Создаём объект для работы с CC1101 по SPI
CC1101 radio(SPI, CC1101_CS_PIN);

void setup() {
  // Указываем режим работы пина CS для CC1101
  pinMode(CC1101_CS_PIN, OUTPUT);

  // Включение Serial для вывода данных
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  // Инициализация SPI
  SPI.begin();
  
  /* Инициализация CC1101 с параметрами по умолчанию:
            Частота: 433.5 МГц
            Модуляция: 2FSK
            Скорость: 1.5 КБ/с
  */
  if (radio.Init() != 0) {
    // При ошибке инициализации - выводим сообщение
    Serial.println(F("Chip not found!"));
    while (true) { delay(1000); }
  }

  // Выставляем частоту работы 433.8 МГц
  radio.setFrequency(433.8);
}

int counter = 0;

void loop() {
  // Создаём сообщение
  String data = "Hello #" + String(counter++);

  // Выводим отправляемое сообщение 
  Serial.print("Transmitting: ");
  Serial.print(data);
  Serial.print(" ");

  // Пробуем передать сообщение через СС1101
  CC1101::Status status = radio.transmit((uint8_t *)data.c_str(), data.length());

  // Проверяем отправку и выводим результат
  if (status == CC1101::STATUS_OK) {
    Serial.println("[OK]");
  } else {
    Serial.print("[ERROR ");
    Serial.print(status);
    Serial.println("]");
  }

  // Ждём 1 секунду
  delay(1000);
}
