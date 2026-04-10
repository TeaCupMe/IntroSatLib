#ifndef FSC_BT986_
#define FSC_BT986_

#include "Adapter/GPIO.h"
#include "Adapter/UART.h"
#include "Adapter/System.h"
#include "Device/ATDevice.h"


namespace IntroSatLib
{

/**
 * \~russian @brief Класс драйвера Bluetooth-модуля FSC-BT986
 * 
 * \~russian Обеспечивает функционал для работы с Bluetooth-модулем FSC-BT986
 */
class FSC_BT986 : public ATDevice
{
public:

    /**
     * \~russian @brief Структура, описывающая пины модуля
     * 
     * \~russian Содержит GPIO для управления модулем FSC-BT986:
     * \~russian - reset: пин сброса
     * \~russian - status: пин индикации статуса
     * \~russian - disconnect: пин принудительного отключения
     * \~russian - mode: пин переключения режима
     */
    struct FSC_BT986Pins
    {
        interfaces::GPIO reset = interfaces::GPIO();       /**< \~russian Пин сброса модуля */
        interfaces::GPIO status = interfaces::GPIO();      /**< \~russian Пин индикации статуса */
        interfaces::GPIO disconnect = interfaces::GPIO();  /**< \~russian Пин принудительного отключения */
        interfaces::GPIO mode = interfaces::GPIO();        /**< \~russian Пин выбора режима работы */
    };

    /**
     * \~russian @brief Режимы работы bluetooth модуля
     */
    enum class Mode: uint8_t
    {
        Throughput = 0, /**< \~russian Режим передачи данных */
        Command = 1     /**< \~russian Командный режим (AT-команды) */
    };

    /**
     * \~russian @brief Способ переключения режима работы
     */
    enum class ModeChange: uint8_t
    {
        None = 0b00,                    /**< \~russian Нет переключения */
        Hardware = 0b01,                /**< \~russian Только аппаратное (пин mode) */
        Software = 0b10,                /**< \~russian Только программное (AT-команда) */
        HardwareAndSoftware = 0b11    /**< \~russian Оба способа */
    };

private:
    FSC_BT986Pins pins; /**< \~russian Структура с пинами модуля */

    ModeChange modeChange = ModeChange::None; /**< \~russian Выбранный способ переключения режима */
    Mode currentMode = Mode::Command;           /**< \~russian Текущий режим работы модуля */

    /** \~russian Время удержания пина по умолчанию (мс) */
    static constexpr uint16_t defaultPinTimeout = 20;

    /**
     * \~russian @brief Программное переключение режима работы (через AT-команду)
     * 
     * \~russian @param state Режим работы модуля (0 - throughput, 1 - command)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetModePrg(Mode mode, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Аппаратное переключение режима работы (через пин mode)
     * 
     * \~russian @param state Состояние пина mode
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetModeHard(Mode mode);

protected:

public:

    /**
     * \~russian @brief Конструктор класса FSC_BT986
     * 
     * \~russian @param uart Ссылка на объект UART интерфейса для связи с модулем
     * \~russian @param _pins Структура с пинами для аппаратного управления модулем
     * \~russian @param bsize Размер буфера для AT-команд/приема данных
     */
    FSC_BT986(
        interfaces::UART uart,
        FSC_BT986Pins _pins,
        const uint16_t bsize = defaultBSize
    ): ATDevice(uart, bsize), pins(_pins)
    { };

    /**
     * \~russian @brief Инициализация Bluetooth-модуля с указанием начального режима
     * 
     * \~russian Выполняет необходимую последовательность действий для
     * \~russian корректного запуска и настройки модуля.
     * 
     * \~russian @param mode Начальный режим работы модуля
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Init(Mode mode);

    /**
     * \~russian @brief Инициализация Bluetooth-модуля в режиме передачи данных
     * 
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Init() override { return Init(Mode::Throughput); }


     /**
     * \~russian @brief Установка способа переключения режимов
     * 
     * \~russian @param newModeChange Способ переключения
     */
    void SetModeChange(ModeChange newModeChange) { modeChange = newModeChange; }

    /**
     * \~russian @brief Получение текущего способа переключения режимов
     * 
     * \~russian @return Текущий способ переключения
     */
    ModeChange GetModeChange() { return modeChange; }

    /**
     * \~russian @brief Переключение режима работы модуля
     * 
     * \~russian Выполняет переключение в зависимости от установленного способа (modeChange)
     * 
     * \~russian @param mode Новый режим работы
     * \~russian @param withSave требуется ли запоминать новое состояние
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetMode(Mode mode, bool withSave=true);

    /**
     * \~russian @brief Установка PIN-кода для Bluetooth-соединения
     * 
     * \~russian @param pinCode Указатель на строку с PIN-кодом
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetPIN(uint8_t* pinCode, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Чтение текущего PIN-кода модуля
     * 
     * \~russian @param rxbuff Указатель на буфер для приема PIN-кода (до 30 байт)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadPIN(uint8_t* rxbuff, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Чтение MAC-адреса Bluetooth-модуля
     * 
     * \~russian @param rxbuff Указатель на буфер для приема MAC-адреса (до 28 байт)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadMAC(uint8_t* rxbuff, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Подключение к другому Bluetooth-устройству по MAC-адресу
     * 
     * \~russian @param mac Указатель на строку с MAC-адресом целевого устройства
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Connect(uint8_t* mac, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Освобождение всех активных соединений
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReleaseConnections(uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Программная перезагрузка модуля (через AT-команду)
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Reboot(uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Аппаратная перезагрузка модуля (через пин reset)
     * 
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef HardReset();

    /**
     * \~russian @brief Чтение статуса модуля через пин status
     * 
     * \~russian @param rxbuff указатель на буфер для приема значения
     * \~russian @return Текущее состояние пина status
     */
    ISL_StatusTypeDef ReadStatus(uint8_t* rxbuff);

    /**
     * \~russian @brief Аппаратное принудительное отключение (через пин disconnect)
     * 
     * \~russian @param state Состояние пина disconnect (0 - /, 1 - disconnected)
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef HardDisconnect();

    /**
     * \~russian @brief Сброс до заводских настроек модуля (требуется Reboot)
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Restore(uint16_t timeout=defaultTimeout);
};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */
