// Подключение библиотек
#include <Wire.h>
#include <AccelerometerV2.h>
#include <IS_Bluetooth.h>

/**
 * Подключение пространства имён библиотеки,
 * чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

// Создание объекта акселерометра
AccelerometerV2 accel(Wire);

// Переменные для хранения показаний датчика
float ax = 0, ay = 0, az = 0;

// Начальный диапазон измерения 2g (+- 20 м/с^2)
Accelerometer::Scale currentScale = Accelerometer::Scale::twoG;

void setup()
{
    // Включение Serial для вывода данных
    Serial.begin(115200, SERIAL_8E1);

    // Инициализация Wire - I2C1
    Wire.begin();

    // Инициализация датчика с диапазоном измерения 2g (стандартная настройка)
    uint8_t status = accel.Init(currentScale);

	/** Проверка успешной инициализации. 
	 *  	0 - Инициализция успешна
	 *		1-3 - Ошибка инициализации 
	 */
	if (status != 0)
    {
        Serial.print("Ошибка инициализации акселерометра! Код ошибки: ");
        Serial.println(status);
		Serial.println("Проверьте подключение и адрес датчика!");
		// Если датчик не инициализирован - уходим в бесконечный цикл
        while (1)
            ;
    }
}

// Бесконечный цикл - основной код программы
void loop()
{
    // Считывание данных с акселерометра
    ax = accel.X();
    ay = accel.Y();
    az = accel.Z();

    // Выводим данные
    Serial.print("ax:");
	Serial.print(ax);
	Serial.print(",ay:");
	Serial.print(ay);
	Serial.print(",az:");
	Serial.println(az);

    // Находим максимальный модуль ускорения по одной из осей
    float mx = max(max(abs(ax), abs(ay)), abs(az));

    // Начальный диапазон измерения акселерометра
    Accelerometer::Scale optimalScale = Accelerometer::Scale::twoG;

    // Если значение стоновится больше прогового, изменяем диапазон измерения на больший.
    // Если значение больше 1.5g - переключаемся на диапазон +-4g
    if (mx > 1.5)
    {
        optimalScale = Accelerometer::Scale::fourG;
    }

    // Если значение больше 3.5g - переключаемся на диапазон +-8g
    else if (mx > 3.5)
    {
        optimalScale = Accelerometer::Scale::eightG;
    }

    // Если значение больше 7.5g - переключаемся на диапазон +-16g
    else if (mx > 7.5)
    {
        optimalScale = Accelerometer::Scale::sixteenG;
    }

    // Изменяем разрешение, если оно не совпадает с установленным
    if (optimalScale != currentScale)
    {
        accel.SetScale(optimalScale);
        currentScale = optimalScale;
    }

    // Проверяем, не пришёл ли запрос на переход в режим перепрошивки
    if (Serial.available())
    {
        if (Serial.read() == 'b')
        {
            // Если пришёл символ 'b', переходим в режим перепрошивки
            enter_bootloader();
        }
    }

    // Задержка в 20 мс, чтобы данные приходили с небольшим перерывом
    delay(20);
}
