# IntroSatLib

Это библиотека для работы с продуктами компании Образование Будущего - образовательными конструкторами спутников:
- IntroSat
- IntroSat.Pico
- IntroSat.Platform

## Оглавление

- [Установка](#установка)
- [CMake](#cmake)
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

## CMake

Для CMake требуется версия 3.20 или новее. Библиотека использует C++17 и
поддерживает два backend: `ARDUINO` и `STM32_HAL`. Одновременно можно выбрать
только один backend.

`IntroSatLib::IntroSatLib` — INTERFACE-агрегатор всех компонентов. Чтобы
собирать только нужные драйверы, можно линковать отдельные STATIC targets:

- `IntroSatLib::Core` — адаптеры платформы, системные функции и логирование;
- `IntroSatLib::Device` — общие I2C/SPI/UART/AT классы устройств;
- `IntroSatLib::Facade` — legacy-фасады верхнего уровня;
- `IntroSatLib::ADS7830`, `IntroSatLib::CC1101`, `IntroSatLib::E32_433`,
  `IntroSatLib::FlyWheel`, `IntroSatLib::FSC_BT986`,
  `IntroSatLib::IRCamera`, `IntroSatLib::LIS2MDL`, `IntroSatLib::LIS3MDL`,
  `IntroSatLib::LM75A`, `IntroSatLib::LSM6DS3`, `IntroSatLib::M24C02`,
  `IntroSatLib::MS5611` и `IntroSatLib::ISLIR*` — отдельные устройства.

Зависимости `Device → Core` и `устройство → Device` подключаются транзитивно.

Arduino Core и STM32 HAL/CMSIS библиотека не скачивает. Родительский проект
прошивки должен заранее создать target, который передаёт необходимые include
directories, compile definitions и библиотеки.

### Подключение через add_subdirectory

Для Arduino target зависимости должен предоставлять как минимум `Arduino.h`,
`Wire.h`, `SPI.h` и соответствующие реализации:

```cmake
# Target создаётся CMake-интеграцией используемого Arduino Core.
add_library(firmware_arduino_framework INTERFACE)
target_link_libraries(
	firmware_arduino_framework
	INTERFACE ArduinoCore Wire SPI
)

set(INTROSATLIB_BACKEND ARDUINO CACHE STRING "" FORCE)
set(
	INTROSATLIB_ARDUINO_TARGET
	firmware_arduino_framework
	CACHE STRING ""
	FORCE
)
add_subdirectory(external/IntroSatLib)

target_link_libraries(firmware PRIVATE IntroSatLib::IntroSatLib)
```

Вместо агрегатора можно выбрать только нужные драйверы:

```cmake
target_link_libraries(firmware PRIVATE
	IntroSatLib::LIS2MDL
	IntroSatLib::LSM6DS3
)
```

Для STM32 target зависимости должен предоставлять HAL/CMSIS headers,
chip-specific define (например, `STM32F103xB`), `USE_HAL_DRIVER` и включённые
HAL-модули:

```cmake
# Обычно этот target создаётся CMake-проектом, сгенерированным CubeMX.
add_library(firmware_stm32_hal INTERFACE)
target_include_directories(firmware_stm32_hal INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc"
	"${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc"
	"${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include"
)
target_compile_definitions(firmware_stm32_hal INTERFACE
	USE_HAL_DRIVER
	STM32F103xB
)

set(INTROSATLIB_BACKEND STM32_HAL CACHE STRING "" FORCE)
set(
	INTROSATLIB_STM32_HAL_TARGET
	firmware_stm32_hal
	CACHE STRING ""
	FORCE
)
add_subdirectory(external/IntroSatLib)

target_link_libraries(firmware PRIVATE IntroSatLib::IntroSatLib)
```

### Установка и find_package

После конфигурации в составе родительского проекта пакет можно установить:

```sh
cmake --build build
cmake --install build --prefix /path/to/prefix
```

Установленный пакет запоминает выбранный backend и точное имя framework-target.
Потребитель должен создать target с тем же именем до `find_package`:

```cmake
add_library(firmware_stm32_hal INTERFACE)
# Настройка HAL/CMSIS target аналогична примеру выше.

find_package(IntroSatLib 0.2 CONFIG REQUIRED)
target_link_libraries(firmware PRIVATE IntroSatLib::IntroSatLib)
```

### Документация

При установленных Doxygen и Graphviz можно добавить target документации:

```cmake
set(INTROSATLIB_BUILD_DOCS ON CACHE BOOL "" FORCE)
# ...выбор backend и framework-target...
add_subdirectory(external/IntroSatLib)
```

Документация генерируется командой `cmake --build build --target docs` в
`doxygen/gen`.

Sketches из `examples/` и `legacy_codes/` продолжают собираться Arduino CLI и
STM32CubeIDE. Они не являются CMake targets, поскольку требуют конкретных
Arduino board packages либо сгенерированного CubeMX-проекта.

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