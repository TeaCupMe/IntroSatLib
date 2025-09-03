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
AccelerometerV2 accel(Wire, 0x6B);

void setup()
{
	// Включение Serial для вывода данных
	Serial.begin(115200, SERIAL_8E1);

	// Инициализация Wire - I2C1
	Wire.begin();

	// Инициализация датчика
	uint8_t status = accel.Init();

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
	// Выводим данные и названия в бесконечном цикле
	Serial.print("ax:");
	Serial.print(accel.X());
	Serial.print(",ay:");
	Serial.print(accel.Y());
	Serial.print(",az:");
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
