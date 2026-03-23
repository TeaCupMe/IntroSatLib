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

private:
    FSC_BT986Pins pins; /**< \~russian Структура с пинами модуля */

    /** \~russian Время удержания пина по умолчанию */
    static constexpr uint16_t DEFAULT_PIN_TIMEOUT = 20;


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
        const uint16_t bsize
    ): ATDevice(uart, bsize), pins(_pins) { }

    /**
     * \~russian @brief Инициализация Bluetooth-модуля
     * 
     * \~russian Выполняет необходимую последовательность действий для
     * \~russian корректного запуска и настройки модуля.
     * 
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Init();

    /**
     * \~russian @brief Установка PIN-кода для Bluetooth-соединения
     * 
     * \~russian @param pinCode Указатель на строку с PIN-кодом
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef writePIN(uint8_t* pinCode, uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Чтение текущего PIN-кода модуля
     * 
     * \~russian @param rxbuff Указатель на буфер для приема PIN-кода (до 30 байт)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef readPIN(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Чтение MAC-адреса Bluetooth-модуля
     * 
     * \~russian @param rxbuff Указатель на буфер для приема MAC-адреса (до 28 байт)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef readMAC(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Подключение к другому Bluetooth-устройству по MAC-адресу
     * 
     * \~russian @param mac Указатель на строку с MAC-адресом целевого устройства
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef connect(uint8_t* mac, uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Программная перезагрузка модуля (через AT-команду)
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef reboot(uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Аппаратная перезагрузка модуля (через пин reset)
     * 
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef hardReset();

    /**
     * \~russian @brief Программное переключение режима работы (через AT-команду)
     * 
     * \~russian @param state Режим работы модуля (0 - througput, 1 - command)
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef setPrgMode(uint8_t state, uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Аппаратное переключение режима работы (через пин mode)
     * 
     * \~russian @param state Состояние пина mode
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef setHardMode(uint8_t state);

    /**
     * \~russian @brief Чтение статуса модуля через пин status
     * 
     * \~russian @param rxbuff указатель на буфер для приема значения
     * \~russian @return Текущее состояние пина status
     */
    ISL_StatusTypeDef readStatus(uint8_t* rxbuff);

    /**
     * \~russian @brief Аппаратное принудительное отключение (через пин disconnect)
     * 
     * \~russian @param state Состояние пина disconnect (0 - /, 1 - disconnected)
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef hardDisconnect();

    /**
     * \~russian @brief Освобождение всех активных соединений
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef releaseConnections(uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief Сброс до заводских настроек модуля (требуется reboot)
     * 
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef restore(uint16_t timeout=DEFAULT_TIMEOUT);
};
    
} // namespace IntroSatLib

#endif /* FSC_BT986_ */