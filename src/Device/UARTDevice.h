#ifndef UARTDEVICE_H_
#define UARTDEVICE_H_

#include "Adapter/UART.h"
#include "Device.h"

namespace IntroSatLib {

/**
 * \~russian @brief Базовый класс для устройств, использующих UART интерфейс
 * 
 * \~russian Класс UARTDevice предоставляет базовую функциональность для работы с устройствами,
 * \~russian подключенными через UART.
 */
class UARTDevice : public Device {
private:
    interfaces::UART _uart; /**< \~russian Объект UART интерфейса для коммуникации с устройством */

protected:

    /** \~russian Таймаут по умолчанию (100 мс) */
    static constexpr uint16_t defaultTimeout = 100;

public:
    /**
     * \~russian @brief Конструктор класса UARTDevice
     * \~russian @param uart Ссылка на объект UART интерфейса
     */
    UARTDevice(interfaces::UART uart) : _uart(uart) {}

    /**
     * \~russian @brief Конструктор копирования
     * \~russian @param other Ссылка на копируемый объект UARTDevice
     */
    UARTDevice(const UARTDevice& other) : _uart(other._uart) {}

    /**
     * \~russian @brief Конструктор перемещения
     * \~russian @param other Ссылка на перемещаемый объект UARTDevice
     */
    UARTDevice(UARTDevice&& other) : _uart(other._uart) {}

    /**
     * \~russian @brief Отправка данных через UART
     * 
     * \~russian @param buf Указатель на буфер с данными для отправки
     * \~russian @param nBytes Количество байт для отправки
     * \~russian @param timeout Таймаут операции в миллисекундах (по умолчанию defaultTimeout)
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef WriteUART(uint8_t* buf, uint8_t nBytes, uint16_t timeout = defaultTimeout);

    /**
     * \~russian @brief Чтение данных из UART
     * 
     * \~russian @param buf Указатель на буфер для приема данных
     * \~russian @param nBytes Количество байт для чтения (по умолчанию 1)
     * \~russian @param timeout Таймаут операции в миллисекундах (по умолчанию defaultTimeout)
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadUART(uint8_t* buf, uint8_t nBytes = 1, uint16_t timeout = defaultTimeout);

    /**
     * \~russian @brief Проверка наличия данных в приемном буфере UART
     * \~russian @return Количество доступных для чтения байт
     */
    uint8_t available();
};

} /* namespace IntroSatLib */

#endif /* UARTDEVICE_H_ */