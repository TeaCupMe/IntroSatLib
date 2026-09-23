# IntroSatLib

Это библиотека для работы с продуктами компании Образование Будущего - образовательными конструкторами спутников:
- IntroSat
- IntroSat.Pico
- IntroSat.Platform

## Оглавление

- [Установка](#установка)
- [Использование](#использование)
- [Полезные ссылки](#полезные-ссылки)

## Установка 

### ArduinoIDE
В [**ArduinoIDE**](https://www.arduino.cc/en/software) библиотеку можно установить одни из двух спобов:

#### Способ 1. Установка через Менеджер Бибилиотек
Шаг 1. Запустите Arduino IDE.

Шаг 2. Откройте **Менеджер Библиотек**

> [!IMPORTANT] 
> В Arduino IDE 2.x **Менеджер Библиотек** можно открыть через боковую панель.
> В Arduino IDE 1.x **Менеджер Библиотек** можно открыть только через меню `Инструменты` -> `Управление Библиотеками`

Шаг 3. В поле поиска введите `IntroSatLib`

Шаг 4. Нажмите **Установка**

#### Способ 2. Установка из архива
Шаг 1. Скачайте библиотеку в `.zip` формате. 
 
Шаг 2. Запустите Arduino IDE.

Шаг 3. В меню `Скетч` -> `Подключить библиотеку` -> `Добавить .ZIP библиотеку...` выберете скачанный `.zip` файл.

### STM32CubeIDE
Шаг 1. Скачайте библиотеку в `.zip` формате и разархивируйте. 

Шаг 2. Переместите папку `IntroSatLib/` в корневую папку проекта STM32CubeIDE (рядом с файлами `**.ioc`, `.cproject`).

Шаг 3. Откройте раздел настройек проекта `Properties -> C/C++ General -> Paths and Symbols`

Шаг 4. Во вкладке `Includes` добавьте папку `IntroSatLib/src/` через кнопку `Add...`. Не забудьте поставить галочки `Add to all languages` и `Add to all configurations`.

Шаг 5. Во вкладке `Source Locations` добавьте папку `IntroSatLib/src/` через кнопку `Add...`

## Использование

**ArduinoIDE:** базовое использование библиотеки

```cpp
    /** Подключение библиотек */
    #include <Wire.h>
    #include <IntroSatLib.h>
    #include <AccelerometerV2.h>

    /*
     * Включение пространства имён библиотеки спутника
     * Чтобы постоянно не писать IntroSatLib::
    */
    using namespace IntroSatLib;

    /** Создание класса акселерометра */
    AccelerometerV2 accel(Wire);

    void setup() {
        Serial.begin(9600); // Включения Serial для вывода данных
        Wire.begin(); // Включение Wire(I2C)
        accel.Init(); // Инициализация датчика
        delay(1000);
        Serial.println("ax, ay, az"); // Для работы плоттера в ArduinoIDE
    }

    /** Вывод данных в бесконечном цикле*/
    void loop() {
        Serial.print(accel.X());
        Serial.print(" ");
        Serial.print(accel.Y());
        Serial.print(" ");
        Serial.println(accel.Z());
        delay(100);
    }
```

## Полезные ссылки

Наш [канал в Telegram](https://t.me/introsat_news) поможет не пропустить обновления.
 
Если у вас возникли вопросы или сложности при работе с IntroSat, ответы можно найти в нашем [F.A.Q.](https://docs.google.com/document/d/15KqFrMlc6Jzxut_zMf_pXNx5r5JTjqfKEvCHWx99rEc/edit#heading=h.demjj79bt080)

Остались вопросы? Напишите нашему [боту в Telegram](https://t.me/introsatBot)! Укажите в обращении модуль конструктора, при работе с которым возникли проблемы, и версии платы (написаны на самих платах), и мы обязательно вам поможем.


<p align="center">
 <img width=70% alt="IntroSat Logo" src="https://github.com/user-attachments/assets/966f7746-2764-4479-848c-38e5ab825ff4"/>
 <!-- <img width=20% alt="Education of the Future" src="https://github.com/user-attachments/assets/1c33d94c-cfc8-4a9b-a658-43fcf6d78393"/> -->
</p>