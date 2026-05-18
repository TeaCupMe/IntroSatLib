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
     * @brief Получение значения напряжения на канале в вольтах
     * 
     * @param channel канал АЦП
     * @return float значение напряжения в вольтах
     */
	float GetValue(uint8_t channel) {
        if (channel >= channelCount) return 0;
        PollChannel(channel);
        return bitsToVolts(values[channel]) * coefficients[channel];
    }

    /**
     * @brief Получение необработанного значения с канала АЦП
     * 
     * @param channel канал АЦП
     * @return uint8_t необработанное значение с канала АЦП
     */
    uint32_t GetRawValue(uint8_t channel) {
        if (channel >= channelCount) return 0;
        PollChannel(channel);
        return values[channel];
    };

    /**
     * @brief Инициализация канала АЦП
     * 
     * @param channel канал АЦП
     * @param coefficient коэффициент преобразования
     * @return ISL_StatusTypeDef результат инициализации
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