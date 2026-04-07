#ifndef E32_433_H_
#define E32_433_H_

#include "Device/UARTDevice.h"
#include "Adapter/GPIO.h"

namespace IntroSatLib
{
    
/**
 * \~russian @brief Класс для управления LoRa модулем E32-433
 * 
 * \~russian Реализует функционал работы с трансивером E32-433 через UART интерфейс.
 * \~russian Позволяет настраивать параметры, передавать и принимать данные.
 */
class E32_433 : public UARTDevice
{
public:

    /**
     * \~russian @brief Структура, описывающая пины управления модулем
     * 
     * \~russian Содержит GPIO пины для управления режимами и контроля статуса:
     * \~russian - M0: пин выбора режима 0
     * \~russian - M1: пин выбора режима 1
     * \~russian - AUX: пин индикации состояния модуля
     */
    struct E32Pins
    {
        interfaces::GPIO M0;
        interfaces::GPIO M1;
        interfaces::GPIO AUX;
    };

    /**
     * \~russian @brief Перечисление режимов работы модуля
     * 
     * \~russian Определяет доступные режимы работы E32-433:
     * \~russian - Normal: нормальный режим (M0=0, M1=0)
     * \~russian - Wakeup: режим пробуждения (M0=1, M1=0)
     * \~russian - PowerSave: энергосберегающий режим (M0=0, M1=1)
     * \~russian - Sleep: спящий режим (M0=1, M1=1)
     */
    enum class Mode: uint8_t
    {
        Normal = 0,
        Wakeup = 1,
        PowerSave = 2,
        Sleep = 3
    };

    /**
     * \~russian @brief Структура настроек модуля E32-433
     * 
     * \~russian Содержит параметры конфигурации модуля размером 6 байт:
     * \~russian - head: заголовок команды (0xC0 для установки параметров)
     * \~russian - addr: адрес модуля (16 бит)
     * \~russian - sped: настройки скорости UART и эфира
     * \~russian - chan: номер канала (0x00-0x1F)
     * \~russian - option: дополнительные опции (режим адресации, вывод, FEC, мощность)
     */
    typedef struct __attribute__((packed)) E32Settings
    {
        uint8_t head = 0xC0;
        uint16_t addr = 0x00;
        uint8_t sped = 0x1A;
        uint8_t chan = 0x17;
        uint8_t option = 0x40;

        E32Settings() { }

        /**
         * \~russian @brief Конструктор из массива байт
         * \~russian @param buff Указатель на массив из 6 байт с настройками
         */
        E32Settings(uint8_t* buff)
        : head(buff[0]), 
          addr(((uint16_t)buff[1]) << 8 | buff[2]), 
          sped(buff[3]), 
          chan(buff[4]), 
          option(buff[5])
        { }

        /**
         * \~russian @brief Конструктор с явным указанием параметров
         * \~russian @param head Заголовок команды
         * \~russian @param addr Адрес модуля
         * \~russian @param sped Байт настроек скорости
         * \~russian @param chan Номер канала
         * \~russian @param option Байт дополнительных опций
         */
        E32Settings(uint8_t head, uint16_t addr, uint8_t sped, uint8_t chan, uint8_t option)
        : head(head), addr(addr), sped(sped), chan(chan), option(option)
        { }

        /**
         * \~russian @brief Оператор присваивания из массива байт
         * \~russian @param bytes Указатель на массив из 6 байт
         * \~russian @return Ссылка на текущий объект
         */
        E32Settings& operator=(uint8_t* bytes)
        {
            head = bytes[0];
            addr = ((uint16_t)bytes[1]) << 8 | bytes[2];
            sped = bytes[3];
            chan = bytes[4];
            option = bytes[5];
            return *this;
        }

    };

    /**
     * \~russian @brief Заголовки команд для работы с регистрами
     * 
     * \~russian Определяет коды команд для взаимодействия с модулем:
     * \~russian - SetParametersSave: установка параметров с сохранением
     * \~russian - GetParameters: чтение параметров
     * \~russian - SetParametersNoSave: установка параметров без сохранения
     * \~russian - GetVersion: чтение версии прошивки
     * \~russian - Reset: программный сброс модуля
     */
    enum class CommandHead : uint8_t
    {
		SetParametersSave = 0xC0,
		GetParameters,
		SetParametersNoSave,
		GetVersion,
		Reset
	};

    /**
     * \~russian @brief Перечисление режимов контроля четности UART
     */
    enum class UARTParity : uint8_t
    {
		None = 0 << 6,    ///< Без контроля четности
        Odd = 0b01 << 6,  ///< Нечетный контроль
        Even = 0b10 << 6  ///< Четный контроль
	};
	
    /**
     * \~russian @brief Перечисление скоростей UART интерфейса
     */
	enum class UARTBaudrate : uint8_t
    {
		BR_1200 = 0,
		BR_2400 = 0b001 << 3,
		BR_4800 = 0b010 << 3,
		BR_9600 = 0b011 << 3,
		BR_19200 = 0b100 << 3,
		BR_38400 = 0b101 << 3,
		BR_57600 = 0b110 << 3,
		BR_115200 = 0b111 << 3
	};
	
    /**
     * \~russian @brief Перечисление скоростей передачи данных в эфире
     */
	enum class AirDatarate : uint8_t
    {
		DR_300 = 0,
		DR_1200 = 0b001,
		DR_2400 = 0b010,
		DR_4800 = 0b011,
		DR_9600 = 0b100,
		DR_19200 = 0b101
	};

    /**
     * \~russian @brief Перечисление режимов адресации
     * 
     * \~russian Определяет способ адресации при передаче:
     * \~russian - Transparent: прозрачный режим (все пакеты принимаются)
     * \~russian - Fixed: фиксированная адресация (только адресные пакеты)
     */
	enum class AddressingMode : uint8_t
    {
		Transparent = 0,
        Fixed = 0b1 << 7
	};
	
    /**
     * \~russian @brief Режимы работы вывода AUX
     */
	enum class IODriveMode : uint8_t
    {
		Opencollector = 0,  ///< Открытый коллектор
        Pushpull = 0b1 << 6 ///< Двухтактный выход
	};
	
    /**
     * \~russian @brief Время пробуждения из спящего режима
     */
	enum class WakeUpTime : uint8_t
    {
		T_250ms = 0,
		T_500ms = 0b001 << 3,
		T_750ms = 0b010 << 3,
		T_1000ms = 0b011 << 3,
		T_1250ms = 0b100 << 3,
		T_1500ms = 0b101 << 3,
		T_1750ms = 0b110 << 3,
		T_2000ms = 0b111 << 3
	};
	
    /**
     * \~russian @brief Включение/отключение FEC (Forward Error Correction)
     */
	enum class FEC : uint8_t
    {
		Disabled = 0,  ///< FEC отключен
        Enabled = 0b1 << 2  ///< FEC включен
	};
	
    /**
     * \~russian @brief Уровни выходной мощности передатчика
     */
	enum class TxPower : uint8_t
    {
		Max = 0,   ///< Максимальная мощность
        Mid = 0b01,  ///< Средняя мощность
        Low = 0b10,  ///< Низкая мощность
        Eco = 0b11   ///< Экономичный режим
	};
    
private:

    static constexpr uint8_t maxChannel = 0x1F;        /**< \~russian Максимальный номер канала */
    static constexpr uint16_t defaultBSize = 1024;     /**< \~russian Размер буфера по умолчанию */
    static constexpr uint16_t txDelay = 350;           /**< \~russian Задержка передачи одного пакета в мс */

    uint32_t lastTransactionTime = 0;          /**< \~russian Время последней транзакции */
    uint32_t transactionCompleteTimeout = 0;   /**< \~russian Таймаут завершения транзакции */

    E32Pins pins;                              /**< \~russian Пины управления модулем */
    E32Settings currentSettings;               /**< \~russian Текущие настройки модуля */
    Mode currentMode = Mode::Normal;           /**< \~russian Текущий режим работы */

    /**
     * \~russian @brief Ожидание заданного уровня на пине AUX
     * \~russian @param level Ожидаемый уровень (0 или 1)
     * \~russian @param timeout Таймаут ожидания в мс
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef WaitAUX(uint8_t level, uint16_t timeout=defaultTimeout);

protected:

    static constexpr uint16_t defaultTimeout = 1000;  /**< \~russian Таймаут по умолчанию в мс */

    /**
     * \~russian @brief Чтение сырых настроек модуля
     * \~russian @param rxbuff Буфер для приема данных
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadSettingsRaw(uint8_t* rxbuff, uint16_t timeout);

public:

    /**
     * \~russian @brief Конструктор класса E32_433
     * \~russian @param uart Интерфейс UART для связи с модулем
     * \~russian @param pins Структура с пинами управления
     */
    E32_433(
        interfaces::UART uart, 
        E32Pins pins
    ): UARTDevice(uart), pins(pins) {}

    /**
     * \~russian @brief Инициализация модуля в нормальном режиме
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Init() { return Init(Mode::Normal); }
    
    /**
     * \~russian @brief Инициализация модуля с указанием режима
     * \~russian @param mode Режим работы модуля
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Init(Mode mode, uint16_t timeout = defaultTimeout);

    /**
     * \~russian @brief Передача данных через LoRa
     * \~russian @param txbuff Указатель на буфер с данными для передачи
     * \~russian @param length Длина передаваемых данных
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef TransmitLoRa(uint8_t* txbuff, uint16_t length, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Прием данных через LoRa
     * \~russian @param rxbuff Указатель на буфер для приема данных
     * \~russian @param length Размер буфера приема данных
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReceiveLoRa(uint8_t* rxbuff, uint16_t length, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка режима работы модуля
     * \~russian @param mode Новый режим работы
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetMode(Mode mode);

    /**
     * \~russian @brief Чтение настроек в массив байт
     * \~russian @param rxbuff Буфер для приема настроек (6 байт)
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadSettings(uint8_t* rxbuff, uint16_t timeout = defaultTimeout);
    
    /**
     * \~russian @brief Чтение настроек в структуру
     * \~russian @param settings Ссылка на структуру для сохранения настроек
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadSettings(E32Settings& settings, uint16_t timeout = defaultTimeout);

    /**
     * \~russian @brief Чтение версии прошивки модуля
     * \~russian @param rxbuff Буфер для приема версии
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef ReadVersion(uint8_t* rxbuff, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Программный сброс модуля
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef Reset(uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка полных настроек модуля
     * \~russian @param settings Структура с новыми настройками
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetSettings(E32Settings settings, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка адреса модуля
     * \~russian @param addr Новый адрес (0x0000-0xFFFF)
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetAddr(uint16_t addr, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка байта настроек скорости SPED
     * \~russian @param sped Байт настроек
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetSPEDByte(uint8_t sped, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Установка режима четности UART
     * \~russian @param parity Режим контроля четности
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetUARTParity(UARTParity parity = UARTParity::None, uint16_t timeout=defaultTimeout);
	
    /**
     * \~russian @brief Установка скорости UART
     * \~russian @param baudrate Скорость UART
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetUARTBaudrate(UARTBaudrate baudrate = UARTBaudrate::BR_9600, uint16_t timeout=defaultTimeout);
	
    /**
     * \~russian @brief Установка скорости передачи в эфире
     * \~russian @param datarate Скорость в эфире
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetAirDatarate(AirDatarate datarate = AirDatarate::DR_2400, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка рабочего канала
     * \~russian @param channel Номер канала (0-31)
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetChannel(uint8_t channel, uint16_t timeout=defaultTimeout);

    /**
     * \~russian @brief Установка байта дополнительных опций OPTION
     * \~russian @param option Байт опций
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetOPTIONByte(uint8_t option, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Установка режима адресации
     * \~russian @param mode Режим адресации
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetFixedTransmission(AddressingMode mode = AddressingMode::Transparent, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Установка режима вывода AUX
     * \~russian @param mode Режим работы вывода
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetIODriveMode(IODriveMode mode = IODriveMode::Opencollector, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Установка времени пробуждения
     * \~russian @param wtime Время пробуждения
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetWakeUpTime(WakeUpTime wtime = WakeUpTime::T_250ms, uint16_t timeout=defaultTimeout);
	
    /**
     * \~russian @brief Включение/отключение FEC
     * \~russian @param fec Состояние FEC
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
	ISL_StatusTypeDef SetFEC(FEC fec = FEC::Enabled, uint16_t timeout=defaultTimeout);
    
    /**
     * \~russian @brief Установка выходной мощности передатчика
     * \~russian @param power Уровень мощности
     * \~russian @param timeout Таймаут операции
     * \~russian @return Статус выполнения операции
     */
    ISL_StatusTypeDef SetTxPower(TxPower power = TxPower::Max, uint16_t timeout=defaultTimeout);

};

} // namespace IntroSatLib

#endif // E32_433_H_