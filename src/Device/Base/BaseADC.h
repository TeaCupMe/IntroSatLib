#ifndef BASE_ADC_H_
#define BASE_ADC_H_

#include "Device/Device.h"
#include "stddef.h"

namespace IntroSatLib {

class BaseADC: public Device {
private: 
    uint8_t channelCount = 0;
    constexpr float bitsToVolts(uint32_t bitValue) {
        return (bitValue * uRef) / (0b1 << (resolution));
    }

    virtual ISL_StatusTypeDef PollChannel(uint8_t channel) = 0;

protected:
    float* coefficients;
    uint32_t* values;
    float uRef;
    uint8_t resolution = 8; // bits
    
    BaseADC(uint8_t _channelCount) {
        channelCount = _channelCount;
        coefficients = new float[_channelCount];
        for (uint8_t i = 0; i < _channelCount; i++) coefficients[i] = 1;
        values = new uint32_t[_channelCount];
    }
    
public:
    /**
     * \~russian @brief Получение значения напряжения на канале в вольтах
     * \~english @brief Get the voltage value on a channel in volts
     * 
     * \~russian @param channel канал АЦП
     * \~english @param channel ADC channel
     * \~russian @return float значение напряжения в вольтах
     * \~english @return float voltage value in volts
     */
	float GetValue(uint8_t channel) {
        if (channel >= channelCount) return 0;
        PollChannel(channel);
        return bitsToVolts(values[channel]) * coefficients[channel];
    }

    /**
     * \~russian @brief Получение необработанного значения с канала АЦП
     * \~english @brief Get the raw value from an ADC channel
     * 
     * \~russian @param channel канал АЦП
     * \~english @param channel ADC channel
     * \~russian @return uint32_t необработанное значение с канала АЦП
     * \~english @return uint32_t raw value from the ADC channel
     */
    uint32_t GetRawValue(uint8_t channel) {
        if (channel >= channelCount) return 0;
        PollChannel(channel);
        return values[channel];
    };

    /**
     * \~russian @brief Инициализация канала АЦП
     * \~english @brief Initialize an ADC channel
     * 
     * \~russian @param channel канал АЦП
     * \~english @param channel ADC channel
     * \~russian @param coefficient коэффициент преобразования
     * \~english @param coefficient conversion coefficient
     * \~russian @return ISL_StatusTypeDef результат инициализации
     * \~english @return ISL_StatusTypeDef initialization result
     */
    ISL_StatusTypeDef InitChannel(uint8_t channel, float coefficient) 
    {
        if (channel >= channelCount) return ISL_StatusTypeDef::ISL_ERROR;
        
        coefficients[channel] = coefficient;
        return ISL_StatusTypeDef::ISL_OK;
    }

    ~BaseADC() {
        delete[] coefficients;
        delete[] values;
    }
};
}

#endif /* BASE_ADC_H_ */