// Подключение библиотек
#include "IntroSatLib.h"
#include "Device/CC1101/CC1101.h"
#include <SPI.h>

// Объявляем пин CS для CC1101
#define CC1101_CS_PIN 4

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

// Создаём переменные
CC1101 radio(SPI, CC1101_CS_PIN); // Объект для работы с CC1101 по SPI
uint8_t buffer[64]; // Буфер для сохранения принятых данных
uint16_t receivedLength = 0; // Переменная для хранения количества принятых байт

void setup() {
    // Указываем режим работы пина CS для CC1101
    pinMode(CC1101_CS_PIN, OUTPUT);

    // Включение Serial для вывода данных
    Serial.begin(115200);
    Serial.println(F("Инициализация..."));
    
    // Инициализация SPI
    SPI.begin();

    /* Инициализация CC1101 с параметрами по умолчанию:
            Частота: 433.5 МГц
            Модуляция: 2FSK
            Скорость: 1.5 КБ/с
    */
    if (radio.Init() != 0) {
        // При ошибке инициализации - выводим сообщение
        Serial.println(F("Ошибка инициализации!"));
        while (true) { delay(1000); }
    }

    // Выставляем частоту работы 433.8 МГц
    radio.setFrequency(433.8);
}

void loop() {
    // Пробуем принять данные
    CC1101::Status status = radio.receive(buffer, 64, &receivedLength);
        
    // При успешном приёме выводим данные
    if (status == CC1101::STATUS_OK) {
        Serial.print("Received ");
        Serial.print(receivedLength);
        Serial.print(" bytes: ");
        Serial.write(buffer, receivedLength);
        Serial.print(" [RSSI: ");
        Serial.print(radio.getRSSI());
        Serial.println(" dBm]");
    }

    // Ждём 1 секунду
    delay(1000);
}
