#ifndef PICO_ADC_H_
#define PICO_ADC_H_

#include "Adapter/I2C.h"
#include "Device/ADS7830/ADS7830.h"

namespace IntroSatLib {
/**
 * @brief Класс для работы с АЦП на плате питания IntroSat.Pico.
 * 
 */
class PicoADC : public ADS7830 {
public:

    /**
     * @brief Каналы АЦП на плате питания IntroSat.Pico.
     * 
     */
    enum Channel: uint8_t {
        CH_VIN =    ADS7830::Channel::CH0, /**< Входное напряжение */
        CH_VBAT =   ADS7830::Channel::CH1, /**< Напряжение на батарее */
        CH_VOUT =   ADS7830::Channel::CH2, /**< Выходное напряжение (после преобразователя) */
        CH_CIN =    ADS7830::Channel::CH3, /**< Входной ток */
        CH_COUT =   ADS7830::Channel::CH4, /**< Выходной ток */
    };

    /**
     * @brief Конструктор объекта АЦП для платы питания IntroSat.Pico
     * 
     * @param i2c объект I2C, например: \c Wire (в Arduino IDE), \c &huart1 (в STM32CubeIDE)
     */
    PicoADC(interfaces::I2C i2c): ADS7830(i2c) {}

    /**
     * @brief Инициализация АЦП для платы питания IntroSat.Pico
     * 
     * @return ISL_StatusTypeDef результат инициализации
     */
    ISL_StatusTypeDef Init() {
        RETURN_STATUS_IF_NOT_OK_SILENT(ADS7830::Init());
        InitChannel(CH_VIN, 4.0f);
        InitChannel(CH_VBAT, 1/0.585f);
        InitChannel(CH_VOUT, 1/0.585f);
        InitChannel(CH_CIN, 1000/6.2f);
        InitChannel(CH_COUT, 1000/6.2f);

        return ISL_StatusTypeDef::ISL_OK;
    }

    /**
     * @brief Получение значения напряжения или тока на канале АЦП платы питания IntroSat.Pico
     * 
     * @param channel Канал АЦП, например: \c CH_VIN, \c CH_VBAT, \c CH_VOUT, \c CH_CIN или \c CH_COUT
     * @return float Значение напряжения (для каналов \c CH_VIN, \c CH_VBAT и \c CH_VOUT) или тока (для каналов \c CH_CIN и \c CH_COUT) в вольтах или миллиамперах соответственно.
     */
    float GetValue(Channel channel) {
        return ADS7830::GetValue((ADS7830::Channel) channel);
    }

    /**
     * @brief Получение входного напряжения(с солнечных панелей) на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVIN() {
        return GetValue(Channel::CH_VIN);
    }

    /**
     * @brief Получение напряжения на батарее на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVBAT() {
        return GetValue(Channel::CH_VBAT);
    }

    /**
     * @brief Получение выходного напряжения на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVOUT() {
        return GetValue(Channel::CH_VOUT);
    }

    /**
     * @brief Получение входного тока(тока зарядки от солнечных панелей) на плате питания IntroSat.Pico
     * 
     * @return float значение тока в миллиамперах
     */
    float GetCIN() {
        return GetValue(Channel::CH_CIN);
    }

    /**
     * @brief Получение выходного тока на плате питания IntroSat.Pico
     * 
     * @return float значение тока в миллиамперах
     */
    float GetCOUT() {
        return GetValue(Channel::CH_COUT);
    }
};

}

#endif /* PICO_ADC_H_ */