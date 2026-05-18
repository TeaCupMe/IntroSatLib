#ifndef ADS7830_H_
#define ADS7830_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseADC.h"

#include "Adapter/System.h"
#include "Adapter/I2C.h"

#include "stdint.h"

namespace IntroSatLib {

/**
 * @brief Класс для работы с 8-канальным 8-битным АЦП ADS7830 по интерфейсу I2C.
 * 
 */

class ADS7830: public BaseADC, protected I2CDevice {
public:
    /**
     * @brief Канал измерения
     */
    enum Channel: uint8_t {
        CH_DIFF_1_TO_0 = 0b0000, /**< Разность напряжений CH1-CH0 */
        CH_DIFF_0_TO_1 = 0b0100, /**< Дифференциальное измерение, CH0-CH1 */
        CH_DIFF_3_TO_2 = 0b0001, /**< Дифференциальное измерение, CH3-CH2 */
        CH_DIFF_2_TO_3 = 0b0101, /**< Дифференциальное измерение, CH2-CH3 */
        CH_DIFF_5_TO_4 = 0b0010, /**< Дифференциальное измерение, CH5-CH4 */
        CH_DIFF_4_TO_5 = 0b0110, /**< Дифференциальное измерение, CH4-CH5 */
        CH_DIFF_7_TO_6 = 0b0011, /**< Дифференциальное измерение, CH7-CH6 */
        CH_DIFF_6_TO_7 = 0b0111, /**< Дифференциальное измерение, CH6-CH7 */
        CH0            = 0b1000, /**< Напряжение на канале CH0 */
        CH1            = 0b1100, /**< Напряжение на канале CH1 */
        CH2            = 0b1001, /**< Напряжение на канале CH2 */
        CH3            = 0b1101, /**< Напряжение на канале CH3 */
        CH4            = 0b1010, /**< Напряжение на канале CH4 */
        CH5            = 0b1110, /**< Напряжение на канале CH5 */
        CH6            = 0b1011, /**< Напряжение на канале CH6 */
        CH7            = 0b1111  /**< Напряжение на канале CH7 */
    };

    /**
     * @brief Опорное напряжение
     */

    enum ReferenceMode: uint8_t {
        REF_EXTERNAL = 0b0, /**< Внешнее опорного напряжение (пин REF<sub>IN</sub>) */
        REF_INTERNAL = 0b1  /**< Внутреннее опорного напряжение (+2.5В) */
    };

    /**
     * @brief Состояние АЦП между измерениями
     */
    enum AdcMode : uint8_t {
        ADC_OFF = 0, /**< АЦП включён */
        ADC_ON  = 1  /**< АЦП выключен (режим power-down) */
    };

private:
    /**
     * @brief Базовый адрес АЦП на шине I2C
     */
    static const uint8_t BASE_ADDRESS = 0x48;

    /**
     * @brief Количество каналов АЦП
     * 
     */
    static const uint8_t channelCount = 16;

    
    ReferenceMode refMode;
    AdcMode adcOn;

    /**
     * @brief Отправка запроса на преобразование значения напряжения на канале 
     * 
     * @param channel канал АЦП
     * @return ISL_StatusTypeDef результат запроса
     */
    ISL_StatusTypeDef RequestConversion(Channel channel);

public:
    /**
     * @brief Конструктор объекта АЦП ADS7830 
     * 
     * @param i2c объект I2C, например: \c Wire (в Arduino IDE), \c &hi2c1 (в STM32CubeIDE)
     * @param address 
     */
    ADS7830(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);
    
    /**
     * @brief Инициализация АЦП с внутренним опорным напряжением
     * 
     * @return ISL_StatusTypeDef результат инициализации
     */
    ISL_StatusTypeDef Init() override;

    /**
     * @brief Инициализация АЦП с внешним опорным напряжением
     * 
     * @param uRef внешнее опорное напряжение (В)
     * @return ISL_StatusTypeDef результат инициализации
     */
    ISL_StatusTypeDef Init(float uRef);

    /**
     * @brief Установка состояния АЦП между измерениями: включён или выключен (режим power-down)
     * 
     * @param on Режим работы АЦП
     */
    void SetAdcOn(AdcMode on);

    /**
     * @brief Использование внешнего опорного напряжения
     * 
     * @param refVoltage внешнее опорное напряжение в вольтах
     * @return ISL_StatusTypeDef результат установки внешнего опорного напряжения
     */
    ISL_StatusTypeDef UseExternalReference(float refVoltage);

    /**
     * @brief Использование внутреннего опорного напряжения
     * 
     * @return ISL_StatusTypeDef результат установки внутреннего опорного напряжения
     */
    ISL_StatusTypeDef UseInternalReference();

    /**
     * @brief Выключение АЦП (режим power-down)
     * 
     * @return ISL_StatusTypeDef результат выключения АЦП
     */
    ISL_StatusTypeDef PowerDown();
    
    /**
     * @brief Получение значения напряжения на канале в вольтах
     * 
     * @param channel канал АЦП
     * @return float значение напряжения в вольтах
     */
    float GetValue(Channel channel);

    /**
     * @brief Получение необработанного значения с канала АЦП
     * 
     * @param channel канал АЦП
     * @return uint8_t необработанное значение с канала АЦП
     */
    uint8_t GetRawValue(Channel channel);
};

}

#endif