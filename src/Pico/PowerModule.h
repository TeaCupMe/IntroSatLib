#ifndef PICO_POWERMODULE_H_
#define PICO_POWERMODULE_H_

#include "Adapter/I2C.h"
#include "Device/ADS7830/ADS7830.h"

namespace IntroSatLib::Pico {
/**
 * \~russian @brief Класс для работы с платой питания IntroSat.Pico.
 * \~english @brief Class for interfacing with the IntroSat.Pico power board.
 */
class PowerModule {
private:
    ADS7830 adc;

public:

    /**
     * \~russian @brief Каналы АЦП на плате питания IntroSat.Pico.
     * \~english @brief ADC channels on the IntroSat.Pico power board.
     */
    enum class AdcChannel : uint8_t {
        CH_VIN =  ADS7830::Channel::CH0, /**< \~russian Входное напряжение 
                                              \~english Input voltage */
        CH_VBAT = ADS7830::Channel::CH1, /**< \~russian Напряжение на батарее 
                                              \~english Battery voltage */
        CH_VOUT = ADS7830::Channel::CH2, /**< \~russian Выходное напряжение (после преобразователя) 
                                              \~english Output voltage */
        CH_CIN =  ADS7830::Channel::CH3, /**< \~russian Входной ток 
                                              \~english Input current */
        CH_COUT = ADS7830::Channel::CH4, /**< \~russian Выходной ток 
                                              \~english Output current */
    };

    /**
     * \~russian @brief Конструктор объекта для работы с платой питания IntroSat.Pico
     * \~english @brief Constructor of the object for interfacing with the IntroSat.Pico power board
     * 
     * \~russian @param i2c объект I2C, например \c Wire или \c Wire1
     * \~english @param i2c I2C object, for example \c Wire or \c Wire1
     */
    PowerModule(interfaces::I2C i2c) : adc(i2c) 
    {}

    /**
     * \~russian @brief Инициализация платы питания IntroSat.Pico
     * \~english @brief Initialize the IntroSat.Pico power board
     * 
     * \~russian @return ISL_StatusTypeDef результат инициализации
     * \~english @return ISL_StatusTypeDef initialization result
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
     * \~russian @brief Получение входного напряжения(с солнечных панелей) на плате питания IntroSat.Pico
     * \~english @brief Get the input voltage (from solar panels) on the IntroSat.Pico power board
     * 
     * \~russian @return float значение напряжения в вольтах
     * \~english @return float voltage value in volts
     */
    float GetVIN() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VIN));
    }

    /**
     * \~russian @brief Получение напряжения на батарее на плате питания IntroSat.Pico
     * \~english @brief Get the battery voltage on the IntroSat.Pico power board
     * 
     * \~russian @return float значение напряжения в вольтах
     * \~english @return float voltage value in volts
     */
    float GetVBAT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VBAT));
    }

    /**
     * \~russian @brief Получение выходного напряжения на плате питания IntroSat.Pico
     * \~english @brief Get the output voltage on the IntroSat.Pico power board
     * 
     * \~russian @return float значение напряжения в вольтах
     * \~english @return float voltage value in volts
     */
    float GetVOUT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_VOUT));
    }

    /**
     * \~russian @brief Получение входного тока(тока зарядки от солнечных панелей) на плате питания IntroSat.Pico
     * \~english @brief Get the input current (charging current from solar panels) on the IntroSat.Pico power board
     * 
     * \~russian @return float значение тока в миллиамперах
     * \~english @return float current value in milliamps
     */
    float GetCIN() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_CIN));
    }

    /**
     * \~russian @brief Получение выходного тока на плате питания IntroSat.Pico
     * \~english @brief Get the output current on the IntroSat.Pico power board
     * 
     * \~russian @return float значение тока в миллиамперах
     * \~english @return float current value in milliamps
     */
    float GetCOUT() {
        return adc.GetValue(static_cast<ADS7830::Channel>(AdcChannel::CH_COUT));
    }
};

}

#endif /* PICO_POWERMODULE_H_ */