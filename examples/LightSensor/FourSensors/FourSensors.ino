// Подключение библиотек
#include <Wire.h>
#include <LightSensor.h>
#include <IS_Bluetooth.h>

/**
 * Подключение пространства имён библиотеки,
 * чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

// Задаём понятные имена для возможных адресов датчиков освещённости
#define LS_ADDR_1 0x50
#define LS_ADDR_2 0x51
#define LS_ADDR_3 0x52
#define LS_ADDR_4 0x53

// Создаём массив из 4-х датчиков освещённости
LightSensor ls[4] = {
    LightSensor(Wire, LS_ADDR_1),
    LightSensor(Wire, LS_ADDR_2),
    LightSensor(Wire, LS_ADDR_3),
    LightSensor(Wire, LS_ADDR_4)
};

void setup()
{
    // Включение Serial для вывода данных
    Serial.begin(115200, SERIAL_8E1);

    // Инициализация Wire - I2C1
    Wire.begin();

    uint8_t status = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        // Инициализация датчика
        status = ls[i].Init();

        /** Проверка успешной инициализации. 
         *  	0 - Инициализция успешна
         *		1-3 - Ошибка инициализации 
         */
        if (status)
        {
            Serial.print("Ошибка инициализации датчика освещённости по адресу 0x");
            Serial.print(LS_ADDR[i], 16);
            Serial.print(". Код ошибки: ");
            Serial.println(status);
            Serial.println("Проверьте подключение датчика.");
            while (1)
                ;
        }
    }
}

void loop()
{
    // Выводим данные и названия в бесконечном цикле
    for (uint8_t i = 0; i < 4; i++)
    {
        Serial.print("light");
        Serial.print(i);
        Serial.print(":")
        Serial.print(ls[i].GetLight());
        Serial.print(',\t');
    }
    Serial.println();

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
