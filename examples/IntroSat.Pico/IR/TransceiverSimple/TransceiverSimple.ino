// Подключаем заголовочные файлы IntroSatLib.
#include <IS_Bluetooth.h>
#include <ISL_Bootloader.h>
// Заголовочный файл класса ИК-приёмопередатчика.
#include <Device/IRTransceiver/IRTransceiver.h>

// Используем пространство имён IntroSatLib, чтобы не писать его перед каждым идентификатором.
using namespace IntroSatLib;

// Создаём объект ИК-приёмопередатчика.
// Пин RX – PD2, пин TX – PD3.
IRTransceiver transceiver(interfaces::GPIO(PD2), interfaces::GPIO(PD3));

// Буфер для приёма данных (максимум 64 байта).
uint8_t rxBuff[64];

// Обработчик прерывания для ИК-приёмника.
// Вызывается при любом изменении сигнала на пине PD2 (INT0).
// Здесь мы просто передаём управление методу ProcessReceiving(),
// который занимается сбором и декодированием ИК-пакетов.
void receiveISR()
{
  transceiver.ProcessReceiving();
}

void setup()
{
  // Инициализируем последовательный порт для связи с компьютером (скорость 9600 бод).
  Serial.begin(9600);
  // Пауза 1 секунда – даём время железу стабилизироваться.
  delay(1000);

  // Инициализируем ИК-приёмопередатчик (настройка GPIO, таймеров и т.п.).
  transceiver.Init();
  delay(1000);

  // Привязываем обработчик прерывания к пину PD2 (INT0).
  // Режим CHANGE – прерывание срабатывает при любом изменении уровня (фронт/спад).
  attachInterrupt(0, receiveISR, CHANGE);
}

void loop()
{
  // ------------------------- ПРИЁМ -------------------------
  // Проверяем, есть ли принятые данные.
  if (transceiver.Available())
  {
    // Забираем до 64 байт в буфер rxBuff, возвращается реальное количество байт.
    uint16_t n = transceiver.GetData(rxBuff, 64);
    // Выводим информацию в монитор последовательного порта.
    Serial.print("Received ");
    Serial.print(n);
    Serial.print(" bytes: ");
    // Печатаем каждый принятый байт как символ и пробел.
    for (uint16_t i = 0; i < n; i++)
    {
      Serial.print((char)rxBuff[i]);
      Serial.print(' ');
    }
    Serial.println();
  }

  // ------------------------- ПЕРЕДАЧА -------------------------
  // Если пользователь что-то отправил через Serial Monitor.
  if (Serial.available())
  {
    // Читаем строку до символа перевода строки.
    String input = Serial.readString();

    // Специальная команда "b" — переход в бутлоадер.
    if (input == "b")
    {
      EnterBootloader();
    }
    else if (input.length() > 0)
    {
      // Готовим массив для передачи – строго по количеству символов,
      // без нуль-терминатора, потому что передаётся "сырая" строка.
      uint8_t txBuff[input.length()];
      // Копируем символы во временный буфер.
      for (unsigned int i = 0; i < input.length(); i++)
      {
        txBuff[i] = (uint8_t)input[i];
      }

      // Информируем пользователя.
      Serial.print("Transmitting: ");
      Serial.println(input);
      // Передаём ИК-посылку с содержимым буфера.
      transceiver.Transmit(txBuff, input.length());
    }
  }
}