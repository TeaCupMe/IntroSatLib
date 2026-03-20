#ifndef ATDEVICE_H_
#define ATDEVICE_H_

#include "Device/UARTDevice.h"

#if __has_include(<cstring>)
    #include <cstring>
#elif __has_include(<string.h>)
    #include <string.h>
#else
    #error "You need to write strlen() yourself :)"
#endif


#define AT_OK       "\r\nOK\r\n"
#define AT_ERROR    "\r\nERROR\r\n"


namespace IntroSatLib {

/**
 * \~russian @brief Базовый класс для устройств, использующих AT-команды
 * 
 * \~russian Класс ATDevice наследуется от UARTDevice и предоставляет функциональность
 * \~russian для формирования и выполнения AT-команд. Поддерживает шаблонные методы
 * \~russian для работы с различным количеством параметров команд.
 */
class ATDevice: public UARTDevice {
private:

protected:
    uint16_t buff_size; /**< \~russian Размер внутреннего буфера для команд */

    /** \~russian Таймаут по умолчанию (500 мс) */
    static constexpr uint16_t DEFAULT_TIMEOUT = 500;
    
    /** \~russian Размер буфера по умолчанию (128 байт) */
    static constexpr uint16_t DEFAULT_BSIZE = 128;

    /**
     * \~russian @brief Виртуальная функция завершения AT-команды
     * 
     * \~russian Может быть переопределена в классах-наследниках для изменения
     * \~russian завершающей последовательности команды.
     * 
     * \~russian @param ptr Указатель на указатель на текущую позицию в буфере
     * \~russian @param end Указатель на указатель на конец буфера
     * \~russian @param nargs Количество аргументов команды
     * \~russian @return Статус выполнения операции
     */
    virtual ISL_StatusTypeDef AT_CMD_END(uint8_t** ptr, uint8_t** end, uint8_t nargs);

    /**
     * \~russian @brief Виртуальная функция разделителя аргументов AT-команды
     * 
     * \~russian Может быть переопределена в классах-наследниках для изменения
     * \~russian символа-разделителя между аргументами.
     * 
     * \~russian @param ptr Указатель на указатель на текущую позицию в буфере
     * \~russian @param end Указатель на указатель на конец буфера
     * \~russian @param nargs Количество аргументов команды
     * \~russian @return Статус выполнения операции
     */
    virtual ISL_StatusTypeDef AT_CMD_SEP(uint8_t** ptr, uint8_t** end, uint8_t nargs);

    /**
     * \~russian @brief Виртуальная функция символа равенства AT-команды
     * 
     * \~russian Может быть переопределена в классах-наследниках для изменения
     * \~russian символа, отделяющего команду от аргументов.
     * 
     * \~russian @param ptr Указатель на указатель на текущую позицию в буфере
     * \~russian @param end Указатель на указатель на конец буфера
     * \~russian @param nargs Количество аргументов команды
     * \~russian @return Статус выполнения операции
     */
    virtual ISL_StatusTypeDef AT_CMD_EQL(uint8_t** ptr, uint8_t** end, uint8_t nargs);

    /**
     * \~russian @brief Виртуальная функция начала AT-команды
     * 
     * \~russian Может быть переопределена в классах-наследниках для изменения
     * \~russian начальной последовательности команды.
     * 
     * \~russian @param ptr Указатель на указатель на текущую позицию в буфере
     * \~russian @param end Указатель на указатель на конец буфера
     * \~russian @param nargs Количество аргументов команды
     * \~russian @return Статус выполнения операции
     */
    virtual ISL_StatusTypeDef AT_CMD_START(uint8_t** ptr, uint8_t** end, uint8_t nargs);

    /**
     * \~russian @brief Выполнение AT-команды
     * 
     * \~russian Отправляет сформированную команду через UART и получает ответ.
     * 
     * \~russian @param command Указатель на буфер с командой
     * \~russian @param rxbuff Указатель на буфер для приема ответа
     * \~russian @param length Размер буфера приема
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef executeATCommand(uint8_t* command, 
                                        uint8_t* rxbuff, 
                                        uint16_t length, 
                                        uint16_t timeout=DEFAULT_TIMEOUT);

public:
    /**
     * \~russian @brief Конструктор класса ATDevice
     * 
     * \~russian @param uart Ссылка на объект UART интерфейса
     * \~russian @param bsize Размер внутреннего буфера (по умолчанию DEFAULT_BSIZE)
     */
    ATDevice(interfaces::UART uart, 
            const uint16_t bsize = DEFAULT_BSIZE) : UARTDevice(uart), 
                                                    buff_size(bsize) {}

    /**
     * \~russian @brief выполнение AT-команды с целью чтения данных
     * 
     * \~russian @param cmd Строка с AT-командой
     * \~russian @param rxbuff Указатель на буфер для приема ответа
     * \~russian @param rxbuff_len Размер буфера приема
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef readAT(const char* cmd, uint8_t* rxbuff, 
                            uint16_t rxbuff_len = DEFAULT_BSIZE, 
                            uint16_t timeout=DEFAULT_TIMEOUT);

    /**
     * \~russian @brief выполнение AT-команды с целью записи данных/выполнения действия
     * 
     * \~russian Шаблонный метод для отправки AT-команд с произвольным
     * \~russian количеством параметров.
     * 
     * \~russian @tparam Args Типы параметров команды
     * \~russian @param cmd Строка с именем AT-команды
     * \~russian @param timeout Таймаут операции в миллисекундах
     * \~russian @param args Параметры команды
     * \~russian @return Статус выполнения операции
     */
    template<typename... Args>
    ISL_StatusTypeDef writeAT(const char* cmd, uint16_t timeout, Args... args) {
        uint8_t buff[buff_size], message[buff_size];

        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD(message, buff_size, cmd, args...));
        
        RETURN_STATUS_IF_NOT_OK_SILENT(executeATCommand(message, buff, buff_size, timeout));
        return (strstr((char*)buff, AT_OK) != nullptr) ? ISL_OK : ISL_ERROR;
    }

    /**
     * \~russian @brief выполнение AT-команды с целью записи 
     * \~russian данных/выполнения действия (с таймаутом по умолчанию)
     * 
     * \~russian @tparam Args Типы параметров команды
     * \~russian @param cmd Строка с именем AT-команды
     * \~russian @param args Параметры команды
     * \~russian @return Статус выполнения операции
     */
    template<typename... Args>
    ISL_StatusTypeDef writeAT(const char* cmd, Args... args) {
        return writeAT(cmd, DEFAULT_TIMEOUT, args...);
    }

    /**
     * \~russian @brief Формирование AT-команды в буфере
     * 
     * \~russian Шаблонный метод для формирования AT-команды с параметрами
     * \~russian в предоставленном буфере.
     * 
     * \~russian @tparam Args Типы параметров команды
     * \~russian @param buff Указатель на буфер для формирования команды
     * \~russian @param buff_size Размер буфера
     * \~russian @param cmd Строка с именем AT-команды
     * \~russian @param args Параметры команды
     * \~russian @return Статус выполнения операции
     */
    template<typename... Args>
    ISL_StatusTypeDef AT_CMD(uint8_t* buff, uint8_t buff_size, const char* cmd, Args... args) {
        uint8_t* ptr = buff;
        uint8_t* end = buff + buff_size;
        constexpr uint8_t num_args = sizeof...(args);
        
        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_START(&ptr, &end, num_args));
        
        uint8_t cmd_len = strlen(cmd);
        if (ptr + cmd_len >= end) return ISL_ERROR;
        memcpy(ptr, cmd, cmd_len);
        ptr += cmd_len;

        RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_EQL(&ptr, &end, num_args));
        if (num_args > 0) {
            const char* params[] = { reinterpret_cast<const char*>(args)... };
            for (uint8_t i = 0; i < num_args; i++) {
                if (i > 0) 
                    RETURN_STATUS_IF_NOT_OK_SILENT(AT_CMD_SEP(&ptr, &end, num_args));
                
                uint8_t param_len = strlen(params[i]);
                if (ptr + param_len >= end) return ISL_ERROR;
                memcpy(ptr, params[i], param_len);
                ptr += param_len;
            }
        }

        return AT_CMD_END(&ptr, &end, num_args);
    }
};

} /* namespace IntroSatLib */

#endif /* ATDEVICE_H_ */
