#ifndef PICO_POWERMODULE_H_
#define PICO_POWERMODULE_H_

#include "Adapter/I2C.h"
#include "Device/ADS7830/ADS7830.h"

namespace IntroSatLib {
/**
 * @brief Класс для работы с платой питания IntroSat.Pico.
 * 
 */
class PowerModule {
private:
    ADS7830 adc;

public:

    /**
     * @brief Каналы АЦП на плате питания IntroSat.Pico.
     * 
     */
    enum class AdcChannel : uint8_t {
        CH_VIN =  ADS7830::Channel::CH0, /**< Входное напряжение */
        CH_VBAT = ADS7830::Channel::CH1, /**< Напряжение на батарее */
        CH_VOUT = ADS7830::Channel::CH2, /**< Выходное напряжение (после преобразователя) */
        CH_CIN =  ADS7830::Channel::CH3, /**< Входной ток */
        CH_COUT = ADS7830::Channel::CH4, /**< Выходной ток */
    };

    /**
     * @brief Конструктор объекта для работы с платой питания IntroSat.Pico
     * 
     * @param i2c объект I2C, например \c Wire или \c Wire1
     */
    PowerModule(interfaces::I2C i2c) : adc(i2c) 
    {}

    /**
     * @brief Инициализация платы питания IntroSat.Pico
     * 
     * @return ISL_StatusTypeDef результат инициализации
     */
    ISL_StatusTypeDef Init() {
        RETURN_STATUS_IF_NOT_OK_SILENT(adc.Init());
        adc.InitChannel(static_cast<uint8_t>(AdcChannel::CH_VIN),   4.0f);
        adc.InitChannel(static_cast<uint8_t>(AdcChannel::CH_VBAT),  1/0.585f);
        adc.InitChannel(static_cast<uint8_t>(AdcChannel::CH_VOUT),  1/0.585f);
        adc.InitChannel(static_cast<uint8_t>(AdcChannel::CH_CIN),   1000/6.2f);
        adc.InitChannel(static_cast<uint8_t>(AdcChannel::CH_COUT),  1000/6.2f);

        return ISL_StatusTypeDef::ISL_OK;
    }

    /**
     * @brief Получение входного напряжения(с солнечных панелей) на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVIN() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VIN));
    }

    /**
     * @brief Получение напряжения на батарее на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVBAT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VBAT));
    }

    /**
     * @brief Получение выходного напряжения на плате питания IntroSat.Pico
     * 
     * @return float значение напряжения в вольтах
     */
    float GetVOUT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VOUT));
    }

    /**
     * @brief Получение входного тока(тока зарядки от солнечных панелей) на плате питания IntroSat.Pico
     * 
     * @return float значение тока в миллиамперах
     */
    float GetCIN() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_CIN));
    }

    /**
     * @brief Получение выходного тока на плате питания IntroSat.Pico
     * 
     * @return float значение тока в миллиамперах
     */
    float GetCOUT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_COUT));
    }
};

}

#endif /* PICO_POWERMODULE_H_ */