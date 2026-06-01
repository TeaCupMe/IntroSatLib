// Подключаем заголовочные файлы IntroSatLib.
#include "IntroSatLib.h"
#include <ISL_Bootloader.h>
#include <Device/ISLIRTransmitter/ISLIRTransmitter.h> // Подключаем заголовочный файл класса ИК-передатчика

using namespace IntroSatLib;       // Пространство имён, чтобы не писать IntroSatLib::

// Создаём объект ИК-передатчика, подключённого к пину PD3.
// Только передача, прием отсутствует.
ISLIRTransmitter transmitter(PD3);

void setup()
{
  // Инициализируем последовательный порт для связи с компьютером (скорость 115200 бод).
  Serial.begin(115200);

  // Инициализируем ИК-передатчик.
  transmitter.Init();
  delay(1000);                 
}

void loop() 
{
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
      transmitter.Transmit(txBuff, input.length());
    }
  }

}
