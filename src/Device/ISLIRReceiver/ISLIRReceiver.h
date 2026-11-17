#ifndef ISLIRRECEIVER_H_
#define ISLIRRECEIVER_H_

#include "Device/Device.h"
#include "Adapter/GPIO.h"
#include "Device/ISLIRProtocol/ISLIRProtocol.h"

namespace IntroSatLib
{

/**
 * \~russian
 * @brief Приёмник ИК-сигналов, работающий по прерываниям.
 *
 * Класс реализует конечный автомат для декодирования ИК-посылок
 * на основе временных параметров протокола. Ожидается вызов
 * ProcessReceiving() в обработчике прерывания по изменению
 * уровня на входном пине.
 */
class ISLIRReceiver : Device
{
public:

protected:

private:
    /**
     * \~russian
     * @brief Состояния конечного автомата приёмника.
     */
    enum class State: uint8_t
    {
        None = 0,     ///< \~russian Не инициализирован
        Idle = 1,     ///< \~russian Ожидание начала посылки
        PreRecv = 2,  ///< \~russian Предварительный приём (обнаружен стартовый импульс)
        RecvSpace = 3,///< \~russian Приём паузы (пробела)
        RecvBit = 4   ///< \~russian Приём бита
    };

    IRProtocol::ProtocolTimings timings = IRProtocol::ProtocolTimings(); ///< \~russian Параметры протокола

    static constexpr uint16_t defaultBSize = 16;          ///< \~russian Размер первичного буфера сырых данных
    static constexpr uint16_t defaultOutputBSize = 64;    ///< \~russian Размер выходного циклического буфера

    State currentState = State::None;   ///< \~russian Текущее состояние
    uint32_t newT = 0;                  ///< \~russian Время последнего изменения (тики таймера)
    uint16_t delta = 0;                 ///< \~russian Длительность последнего интервала
    uint32_t (*TimeSource)() {system::GetTick}; ///< \~russian Функция-источник времени (тики)

    interfaces::GPIO receivePin;        ///< \~russian Пин приёма

    uint16_t rawBuffN = 0;              ///< \~russian Количество элементов в первичном буфере
    uint16_t rawBuff[defaultBSize];     ///< \~russian Первичный буфер длительностей

    volatile uint16_t outputRawBuffHead = 0; ///< \~russian Указатель головы выходного буфера
    volatile uint16_t outputRawBuffTail = 0; ///< \~russian Указатель хвоста выходного буфера
    volatile uint8_t outputRawBuff[defaultOutputBSize]; ///< \~russian Выходной циклический буфер байт
    volatile bool readingDataFlag = false; ///< \~russian Флаг процесса чтения данных (для защиты буфера)

    /**
     * \~russian
     * @brief Помещает байт в выходной циклический буфер.
     * @param value Байт для записи.
     * @return ISL_OK при успешной записи.
     */
    ISL_StatusTypeDef put(uint8_t value);

    /**
     * \~russian
     * @brief Извлекает байт из выходного циклического буфера.
     * @param[out] value Указатель на переменную, в которую будет записан байт.
     * @return ISL_OK при успешном чтении, ISL_ERROR если буфер пуст.
     */
    ISL_StatusTypeDef pop(uint8_t *value);

    /**
     * \~russian
     * @brief Обработчик состояния Idle.
     *
     * Ожидает стартовый импульс (длительность в пределах markStart ± errorScale).
     * При обнаружении переходит в состояние PreRecv.
     */
    void IDLEStateProcess();

    /**
     * \~russian
     * @brief Обработчик состояния PreRecv.
     *
     * Проверяет стартовый пробел. Если длительность пробела соответствует протоколу,
     * переходит к приёму битовых интервалов.
     */
    void PreRecvStateProcess();

    /**
     * \~russian
     * @brief Обработчик состояния RecvSpace.
     *
     * Оценивает длительность паузы, определяет конец бита или конец посылки.
     * При необходимости помещает байты в выходной буфер.
     */
    void RecvSpaceStateProcess();

    /**
     * \~russian
     * @brief Обработчик состояния RecvBit.
     *
     * Оценивает длительность маркера, распознаёт бит (0 или 1) и сохраняет его.
     */
    void RecvBitStateProcess();

public:
    /**
     * \~russian
     * @brief Конструктор.
     * @param _receivePin Пин, на который приходит ИК-сигнал.
     */
    ISLIRReceiver(interfaces::GPIO _receivePin) : receivePin(_receivePin) { }

    /**
     * \~russian
     * @brief Инициализация приёмника (переводит в состояние Idle).
     * @return ISL_OK при успешной инициализации.
     */
    ISL_StatusTypeDef Init()
    {
        currentState = State::Idle;
        return ISL_OK;
    }

    /**
     * \~russian
     * @brief Основной метод обработки, вызывается в обработчике прерывания.
     *
     * Считывает текущее время через TimeSource, вычисляет длительность последнего
     * интервала и вызывает обработчик текущего состояния конечного автомата.
     */
    void ProcessReceiving();

    /**
     * \~russian
     * @brief Установка пользовательского источника времени.
     * @param _TimeSource Указатель на функцию, возвращающую тики (uint32_t).
     */
    void SetTimeSource(uint32_t (*_TimeSource)()) { TimeSource = _TimeSource; }

    /**
     * \~russian
     * @brief Возвращает количество байт, доступных для чтения.
     * @return Число байт в выходном буфере.
     */
    uint16_t Available();

    /**
     * \~russian
     * @brief Чтение принятых данных.
     *
     * Копирует до length байт из внутреннего выходного буфера в пользовательский буфер.
     * @param buff Указатель на буфер-приёмник.
     * @param length Максимальное количество байт для чтения.
     * @return Реально прочитанное количество байт.
     */
    uint16_t GetData(uint8_t* buff, uint16_t length);
};

}

#endif