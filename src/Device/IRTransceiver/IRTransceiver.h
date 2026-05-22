#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#if defined(ARDUINO_AVR_ATmega328)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"

namespace IntroSatLib
{

/**
 * \~russian
 * @brief Приёмо-передатчик ИК-сигналов (трансивер).
 *
 * Объединяет функциональность приёмника ISLIRReceiver и передатчика
 * ISLIRTransmitter, предоставляя единый интерфейс. Позволяет
 * принимать и отправлять данные в одном устройстве.
 */
class IRTransceiver : Device
{
private:
    ISLIRReceiver receiver;         ///< \~russian Экземпляр приёмника
    ISLIRTransmitter transmitter;   ///< \~russian Экземпляр передатчика

public:
    /**
     * \~russian
     * @brief Конструктор трансивера.
     * @param rxPin Пин для приёма ИК-сигналов.
     * @param txPin Пин для передачи ИК-сигналов.
     * @param modulationFreq Частота модуляции передатчика (по умолчанию 38000 Гц).
     */
    IRTransceiver(interfaces::GPIO rxPin, interfaces::GPIO txPin, uint16_t modulationFreq = 38000)
        : receiver(rxPin)
        , transmitter(txPin, modulationFreq)
    { };

    /**
     * \~russian
     * @brief Инициализация трансивера.
     *
     * Последовательно вызывает Init() для приёмника и передатчика.
     * При ошибке инициализации приёмника процесс прерывается.
     * @return ISL_OK при успехе, иначе код ошибки от приёмника.
     */
    ISL_StatusTypeDef Init();

    // ----- Интерфейс приёмника -----

    /**
     * \~russian
     * @brief Обработка принятых импульсов (делегируется receiver.ProcessReceiving()).
     *
     * Должна вызываться в обработчике прерывания по изменению уровня на пине RX.
     */
    void ProcessReceiving();

    /**
     * \~russian
     * @brief Установка пользовательского источника времени для приёмника.
     * @param TimeSource Функция, возвращающая текущее время в тиках.
     */
    void SetTimeSource(uint32_t (*TimeSource)());

    /**
     * \~russian
     * @brief Количество байт, готовых к чтению из приёмника.
     * @return Число байт в буфере приёмника.
     */
    uint16_t Available();

    /**
     * \~russian
     * @brief Чтение принятых данных из приёмника.
     * @param buff Выходной буфер.
     * @param length Максимальное количество байт для чтения.
     * @return Реально прочитанное количество байт.
     */
    uint16_t GetData(uint8_t* buff, uint16_t length);

    // ----- Интерфейс передатчика -----

    /**
     * \~russian
     * @brief Передача массива данных через ИК-передатчик.
     * @param txBuff Указатель на массив байт.
     * @param nbytes Количество байт для передачи.
     * @return ISL_OK при успешной отправке, иначе код ошибки.
     */
    ISL_StatusTypeDef Transmit(uint8_t* txBuff, uint16_t nbytes);

    /**
     * \~russian
     * @brief Назначить функцию Tone (включение несущей) передатчику.
     * @param Tone Указатель на функцию.
     */
    void SetTone(void (*Tone)(interfaces::GPIO tonePin, uint16_t frequency));

    /**
     * \~russian
     * @brief Назначить функцию NoTone (выключение несущей) передатчику.
     * @param NoTone Указатель на функцию.
     */
    void SetNoTone(void (*NoTone)(interfaces::GPIO tonePin));

    /**
     * \~russian
     * @brief Назначить функцию задержки передатчику.
     * @param DelaySource Указатель на функцию.
     */
    void SetDelaySource(void (*DelaySource)(uint32_t time));

    /**
     * \~russian
     * @brief Установить частоту модуляции передатчика.
     * @param freq Частота в Гц.
     */
    void SetModulationFrequence(uint16_t freq);

    /**
     * \~russian
     * @brief Получить текущую частоту модуляции передатчика.
     * @return Частота в Гц.
     */
    uint16_t GetModulationFrequence();
};

} // namespace IntroSatLib

#endif // ARDUINO_AVR_ATmega328
#endif // IRTRANSCEIVER_H_