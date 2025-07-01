// Подключение библиотек
#include <Wire.h>
#include <Accelerometer.h>
#include <IS_Bluetooth.h>

/**
 * Подключение пространства имён библиотеки,
 * чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

// Создание объекта акселерометра
Accelerometer accel(Wire);

// Переменные для хранения показаний датчика
float ax = 0, ay = 0, az = 0;

// Начальный диапазон измерения
Accelerometer::Scale lastScale = Accelerometer::Scale::twoG;

void setup()
{
    // Включение Serial для вывода данных
    Serial.begin(115200, SERIAL_8E1);

    // Инициализация Wire - I2C1
    Wire.begin();

    // Инициализация датчика с диапазоном измерения 2g (стандартная настройка)
    accel.Init(lastScale);
    delay(1000);

    // Выводим названия осей для работы плоттера в ArduinoIDE
    Serial.println("ax, ay, az");
}

void loop()
{
    // Считывание данных с акселерометра
    ax = accel.X();
    ay = accel.Y();
    az = accel.Z();

    // Находим максимальный модуль ускорения по одной из осей
    float mx = max(max(abs(ax), abs(ay)), abs(az));

    // Начальный диапазон измерения акселерометра
    Accelerometer::Scale nowScale = Accelerometer::Scale::twoG;

    // Если значение стоновится больше прогового, изменяем диапазон измерения на больший.
    // Если значение больше 1.5g - переключаемся на диапазон +-4g
    if (mx > 1.5)
    {
        nowScale = Accelerometer::Scale::fourG;
    }

    // Если значение больше 3.5g - переключаемся на диапазон +-8g
    else if (mx > 3.5)
    {
        nowScale = Accelerometer::Scale::eightG;
    }

    // Если значение больше 7.5g - переключаемся на диапазон +-16g
    else if (mx > 7.5)
    {
        nowScale = Accelerometer::Scale::sixteenG;
    }

    // Изменяем разрешение, если оно не совпадает с прошлым
    if (nowScale != lastScale)
    {
        accel.SetScale(nowScale);
        lastScale = nowScale;
    }

    // Выводим данные
    Serial.print(ax);
    Serial.print(" ");
    Serial.print(ay);
    Serial.print(" ");
    Serial.println(az);

    // Проверяем, не пришёл ли запрос на переход в режим перепрошивки
    if (Serial.available())
    {
        if (Serial.read() == 'b')
        {
            // Если пришёл символ 'b', переходим в режим перепрошивки
            enter_bootloader();
        }
    }

    // Задержка в 50 мс, чтобы данные приходили с небольшим перерывом
    delay(50);
}
