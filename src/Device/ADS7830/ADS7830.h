#ifndef ADS7830_H_
#define ADS7830_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseADC.h"

#include "Adapter/System.h"
#include "Adapter/I2C.h"

#include "stdint.h"

namespace IntroSatLib {

/**
 * \~russian @brief Класс для работы с 8-канальным 8-битным АЦП ADS7830 по интерфейсу I2C.
 * \~english @brief Class for interfacing with 8-channel 8-bit ADC ADS7830 via I2C interface.
 */

class ADS7830: public BaseADC, protected I2CDevice {
public:
    /**
     * \~russian @brief Канал измерения
     * \~english @brief Measurement Channel
     */
    enum Channel: uint8_t {
        CH_DIFF_1_TO_0 = 0b0000, /**< \~russian Разность напряжений CH1-CH0 
                                      \~english Differential voltage between CH1 and CH0 */
        CH_DIFF_0_TO_1 = 0b0100, /**< \~russian Дифференциальное измерение, CH0-CH1 
                                      \~english Differential voltage between CH0 and CH1 */
        CH_DIFF_3_TO_2 = 0b0001, /**< \~russian Дифференциальное измерение, CH3-CH2 
                                      \~english Differential voltage between CH3 and CH2 */
        CH_DIFF_2_TO_3 = 0b0101, /**< \~russian Дифференциальное измерение, CH2-CH3 
                                      \~english Differential voltage between CH2 and CH3 */
        CH_DIFF_5_TO_4 = 0b0010, /**< \~russian Дифференциальное измерение, CH5-CH4 
                                      \~english Differential voltage between CH5 and CH4 */
        CH_DIFF_4_TO_5 = 0b0110, /**< \~russian Дифференциальное измерение, CH4-CH5 
                                      \~english Differential voltage between CH4 and CH5 */
        CH_DIFF_7_TO_6 = 0b0011, /**< \~russian Дифференциальное измерение, CH7-CH6 
                                      \~english Differential voltage between CH7 and CH6 */
        CH_DIFF_6_TO_7 = 0b0111, /**< \~russian Дифференциальное измерение, CH6-CH7 
                                      \~english Differential voltage between CH6 and CH7 */
        CH0            = 0b1000, /**< \~russian Напряжение на канале CH0 
                                      \~english Voltage on channel CH0 */
        CH1            = 0b1100, /**< \~russian Напряжение на канале CH1 
                                      \~english Voltage on channel CH1 */
        CH2            = 0b1001, /**< \~russian Напряжение на канале CH2 
                                      \~english Voltage on channel CH2 */
        CH3            = 0b1101, /**< \~russian Напряжение на канале CH3 
                                      \~english Voltage on channel CH3 */
        CH4            = 0b1010, /**< \~russian Напряжение на канале CH4 
                                      \~english Voltage on channel CH4 */
        CH5            = 0b1110, /**< \~russian Напряжение на канале CH5 
                                      \~english Voltage on channel CH5 */
        CH6            = 0b1011, /**< \~russian Напряжение на канале CH6 
                                      \~english Voltage on channel CH6 */
        CH7            = 0b1111  /**< \~russian Напряжение на канале CH7 
                                      \~english Voltage on channel CH7 */
    };

    /**
     * \~russian @brief Опорное напряжение
     * \~english @brief Reference Voltage
     */
    enum ReferenceMode: uint8_t {
        REF_EXTERNAL = 0b0, /**< \~russian Внешнее опорного напряжение (пин REF<sub>IN</sub>) 
                                 \~english External reference voltage (pin REF<sub>IN</sub>) */
        REF_INTERNAL = 0b1  /**< \~russian Внутреннее опорного напряжение (+2.5В) 
                                 \~english Internal reference voltage (+2.5V) */
    };

    /**
     * \~russian @brief Состояние АЦП между измерениями
     * \~english @brief ADC State Between Measurements
     */
    enum AdcMode : uint8_t {
        ADC_OFF = 0, /**< \~russian АЦП включён 
                          \~english ADC is on */
        ADC_ON  = 1  /**< \~russian АЦП выключен (режим power-down) 
                          \~english ADC is off (power-down mode) */
    };

private:
    /**
     * \~russian @brief Базовый адрес АЦП на шине I2C
     * \~english @brief Base address of the ADC on the I2C bus
     */
    static const uint8_t BASE_ADDRESS = 0x48;

    /**
     * \~russian @brief Количество каналов АЦП
     * \~english @brief Number of channels of the ADC
     */
    static const uint8_t channelCount = 16;

    ReferenceMode refMode;
    AdcMode adcOn;

    /**
     * \~russian @brief Отправка запроса на преобразование значения напряжения на канале
     * \~english @brief Send a request to convert the voltage value on the channel
     * 
     * \~russian @param channel канал АЦП
     * \~english @param channel ADC channel
     * \~russian @return ISL_StatusTypeDef результат запроса
     * \~english @return ISL_StatusTypeDef result of the request
     */
    ISL_StatusTypeDef RequestConversion(Channel channel);

    /**
    * \~russian @brief Получение значения с канала АЦП
    * \~english @brief Get value from ADC channel
    * 
    * \~russian @param channel канал АЦП
    * \~english @param channel ADC channel
    * \~russian @return ISL_StatusTypeDef результат получения значения с канала
    * \~english @return ISL_StatusTypeDef result of the request
    */  
    ISL_StatusTypeDef PollChannel(uint8_t channel) override;

public:
    /**
     * \~russian @brief Конструктор объекта АЦП ADS7830 
     * \~english @brief Constructor of the ADS7830 ADC object
     * 
     * \~russian @param i2c объект I2C, например: \c Wire (в Arduino IDE), \c &hi2c1 (в STM32CubeIDE)
     * \~english @param i2c I2C object, for example: \c Wire (in Arduino IDE), \c &hi2c1 (in STM32CubeIDE)
     * \~russian @param address адрес АЦП на шине I2C
     * \~english @param address ADC address on the I2C bus
     */
    ADS7830(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);
    
    /**
     * \~russian @brief Инициализация АЦП с внутренним опорным напряжением
     * \~english @brief Initialize the ADC with internal reference voltage
     * 
     * \~russian @return ISL_StatusTypeDef результат инициализации
     * \~english @return ISL_StatusTypeDef initialization result
     */
    ISL_StatusTypeDef Init() override;

    /**
     * \~russian @brief Инициализация АЦП с внешним опорным напряжением
     * \~english @brief Initialize the ADC with external reference voltage
     * 
     * \~russian @param uRef внешнее опорное напряжение (В)
     * \~english @param uRef external reference voltage (V)
     * \~russian @return ISL_StatusTypeDef результат инициализации
     * \~english @return ISL_StatusTypeDef initialization result
     */
    ISL_StatusTypeDef Init(float uRef);

    /**
     * \~russian @brief Установка состояния АЦП между измерениями: включён или выключен (режим power-down)
     * \~english @brief Set the state of the ADC between measurements: on or off (power-down mode)
     * 
     * \~russian @param on Режим работы АЦП
     * \~english @param on ADC operation mode
     */
    void SetAdcOn(AdcMode on);

    /**
     * \~russian @brief Использование внешнего опорного напряжения
     * \~english @brief Use external reference voltage
     * 
     * \~russian @param refVoltage внешнее опорное напряжение в вольтах
     * \~english @param refVoltage external reference voltage in volts
     * \~russian @return ISL_StatusTypeDef результат установки внешнего опорного напряжения
     * \~english @return ISL_StatusTypeDef result of setting the external reference voltage
     */
    ISL_StatusTypeDef UseExternalReference(float refVoltage);

    /**
     * \~russian @brief Использование внутреннего опорного напряжения
     * \~english @brief Use internal reference voltage
     * 
     * \~russian @return ISL_StatusTypeDef результат установки внутреннего опорного напряжения
     * \~english @return ISL_StatusTypeDef result of setting the internal reference voltage
     */
    ISL_StatusTypeDef UseInternalReference();

    /**
     * \~russian @brief Выключение АЦП (режим power-down)
     * \~english @brief Turn off the ADC (power-down mode)
     * 
     * \~russian @return ISL_StatusTypeDef результат выключения АЦП
     * \~english @return ISL_StatusTypeDef result of turning off the ADC
     */
    ISL_StatusTypeDef PowerDown();
};

}

#endif