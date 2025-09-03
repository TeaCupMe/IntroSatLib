// Подключение библиотек
#include <Wire.h>
#include <GyroscopeV2.h>
#include <IS_Bluetooth.h>

/**
 * Подключение пространства имён библиотеки,
 * чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

// Создание объекта гироскопа
GyroscopeV2 gyro(Wire, 0x6B);

void setup()
{
    // Включение Serial для вывода данных
    Serial.begin(115200, SERIAL_8E1);

    // Инициализация Wire - I2C1
    Wire.begin();

    // Инициализация датчика
    uint8_t status = gyro.Init();

    /** Проверка успешной инициализации. 
	 *  	0 - Инициализция успешна
	 *		1-3 - Ошибка инициализации 
	 */
    if (status != 0)
    {
        Serial.print("Ошибка инициализации гироскопа! Код ошибки: ");
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
    Serial.print("gx:");
	Serial.print(gyro.X());
	Serial.print(",gy:");
	Serial.print(gyro.Y());
	Serial.print(",gz:");
	Serial.println(gyro.Z());

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
