/** Подключение библиотек */
#include <Wire.h>
#include <CoilFlyWheel.h>
#include <IS_Bluetooth.h>

/**
 * Подключение пространства имён библиотеки,
 * чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;


// Создаём объект для управления магнитными катушками.
// 0x3A - адрес устройства, может быть другим
CoilFlyWheel coil(Wire, 0x3A);

void setup()
{
    // Включение Serial для вывода данных
    Serial.begin(115200, SERIAL_8E1);

    // Инициализация Wire - I2C1
    Wire.begin();

    // Ждём 1 секунду, чтобы шина I2C точно иниализировалась
    delay(1000);

    // Пытаемся инициализировать катушки
    Serial.print("Initializing CoilFlyWheel");
    while (coil.Init())
    {
        Serial.print(".");
    }
    
    // Выводим сообщение об успешной инициализации
    Serial.println("\nCoilFlyWheel Initialized!");
}

void loop()
{
    // Изменяем значение, подаваемое на катушки, в диапазоне от -30000 до +30000 с шагом 5000.
    for (int i = -30000; i <= 30000; i += 5000)
    {
        // Устанавливаем значение
        coil.NeedSpeed(i);

        // Выводим сообщение о поданном значении
        Serial.print("CoilFlyWheel speed: ");
        Serial.println(i);

        // Проверяем, не пришёл ли запрос на переход в режим перепрошивки
        if (Serial.available())
        {
            if (Serial.read() == 'b')
            {
                // Если пришёл символ 'b', переходим в режим перепрошивки
                enter_bootloader();
            }
        }

        // Задержка в 1 секунду, чтобы между изменения была задержка 
        delay(1000);
    }
    // Выводим сообщение-разделитель, чтобы разделить циклы изменения скорости в мониторе порта
    Serial.println("======================================")
}
