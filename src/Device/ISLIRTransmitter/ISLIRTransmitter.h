#ifndef ISLIRTRANSMITTER_H_
#define ISLIRTRANSMITTER_H_

#if defined(ARDUINO_AVR_ATmega328)

#include "Device/Device.h"
#include "Adapter/GPIO.h"
#include "Device/ISLIRProtocol/ISLIRProtocol.h"

namespace IntroSatLib
{

/**
 * \~russian
 * @brief Передатчик ИК-сигналов.
 *
 * Формирует модулированные посылки согласно заданным временным
 * параметрам протокола. Использует функции генерации несущей (Tone/NoTone)
 * и задержек.
 */
class ISLIRTransmitter : Device
{

public:

protected:

private:
    interfaces::GPIO txPin;          ///< \~russian Выходной пин передатчика

    IRProtocol::ProtocolTimings timings = IRProtocol::ProtocolTimings(); ///< \~russian Параметры протокола
    uint16_t modulationFrequence;     ///< \~russian Частота модуляции (Гц), по умолчанию 38000

    void (*Tone)(interfaces::GPIO tonePin, uint16_t frequency) { tone };   ///< \~russian Функция включения несущей
    void (*NoTone)(interfaces::GPIO tonePin) { noTone };                   ///< \~russian Функция выключения несущей
    void (*DelaySource)(uint32_t time) {system::Delay};                    ///< \~russian Функция задержки (мкс или мс)

    /**
     * \~russian
     * @brief Генерирует массив длительностей модуляции для передачи битового потока.
     *
     * Преобразует байтовый массив в последовательность интервалов маркер/пробел
     * согласно протоколу. Каждому биту соответствуют два значения (маркер и пробел).
     * @param txBuff Указатель на байты для передачи.
     * @param nBits Количество бит для кодирования.
     * @param[out] buff Выходной буфер для длительностей.
     * @param length Максимальная ёмкость выходного буфера.
     * @return Количество записанных в buff элементов (каждый элемент – длительность в условных единицах).
     */
    uint16_t GenerateRawTxData(uint8_t* txBuff, uint16_t nBits, uint16_t* buff, uint16_t length);

    /**
     * \~russian
     * @brief Отправляет один байт данных.
     *
     * Разбирает байт на отдельные биты и для каждого генерирует
     * необходимую последовательность модуляции.
     * @param byte Байт для отправки.
     * @return ISL_OK при успешной отправке, иначе код ошибки.
     */
    ISL_StatusTypeDef SendByte(uint8_t byte);

public:
    /**
     * \~russian
     * @brief Конструктор.
     * @param _txPin Выходной пин.
     * @param frequence Частота модуляции (по умолчанию 38000 Гц).
     */
    ISLIRTransmitter(interfaces::GPIO _txPin, uint16_t frequence = 38000) : txPin(_txPin),
                                                                            modulationFrequence(frequence) { }
    /**
     * \~russian
     * @brief Инициализация передатчика.
     * @return ISL_OK.
     */
    ISL_StatusTypeDef Init() { return ISL_OK; }

    /**
     * \~russian
     * @brief Передать массив байт.
     *
     * Генерирует модулированную посылку для всего массива данных.
     * @param txBuff Указатель на данные.
     * @param nbytes Количество байт для передачи.
     * @return ISL_OK при успешной передаче, иначе код ошибки.
     */
    ISL_StatusTypeDef Transmit(uint8_t* txBuff, uint16_t nbytes);

    /**
     * \~russian
     * @brief Назначить функцию включения несущей.
     * @param _Tone Указатель на функцию с сигнатурой void(interfaces::GPIO, uint16_t).
     */
    void SetTone(void (*_Tone)(interfaces::GPIO tonePin, uint16_t frequency)) { Tone = _Tone; }

    /**
     * \~russian
     * @brief Назначить функцию выключения несущей.
     * @param _NoTone Указатель на функцию с сигнатурой void(interfaces::GPIO).
     */
    void SetNoTone(void (*_NoTone)(interfaces::GPIO tonePin)) { NoTone = _NoTone; }

    /**
     * \~russian
     * @brief Назначить функцию задержки.
     * @param _DelaySource Указатель на функцию с сигнатурой void(uint32_t).
     */
    void SetDelaySource(void (*_DelaySource)(uint32_t time)) { DelaySource = _DelaySource; }

    /**
     * \~russian
     * @brief Установить частоту модуляции.
     * @param _frequence Частота в Гц.
     */
    void SetModulationFrequence(uint16_t _frequence) { modulationFrequence = _frequence; }

    /**
     * \~russian
     * @brief Получить текущую частоту модуляции.
     * @return Частота в Гц.
     */
    uint16_t GetModulationFrequence() { return modulationFrequence; }
};

}

#endif
#endif