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

void setup() {
	// Включение Serial для вывода данных
  	Serial.begin(115200, SERIAL_8E1);

	// Инициализация Wire - I2C1
  	Wire.begin(); 

	// Инициализация датчика
    accel.Init();
    delay(1000);

	// Выводим названия осей для работы плоттера в ArduinoIDE
    Serial.println("ax, ay, az");
}

// Выводим данные в бесконечном цикле
void loop() {
	Serial.print(accel.X());
	Serial.print(" ");
	Serial.print(accel.Y());
	Serial.print(" ");
	Serial.println(accel.Z());

	// Проверяем, не пришёл ли запрос на переход в режим перепрошивки
    if (Serial.available())
    {
        if (Serial.read() == 'b')
        {
            // Если пришёл символ 'b', переходим в режим перепрошивки
            enter_bootloader();
        }
    }

	// Задержка в 100 мс, чтобы данные приходили с небольшим перерывом
	delay(100);
}
