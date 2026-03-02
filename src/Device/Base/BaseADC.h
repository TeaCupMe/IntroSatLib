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
	float GetValue(uint8_t channel) {
        if (channel >= channelCount) return 0;
        return bitsToVolts(values[channel]) * coefficients[channel];
    }

    ISL_StatusTypeDef InitChannel(uint8_t channel, float coefficient) 
    {
        if (channel >= channelCount) return ISL_StatusTypeDef::ISL_ERROR;
        
        coefficients[channel] = coefficient;
        return ISL_StatusTypeDef::ISL_OK;
    }

    uint32_t* GetValuesPointer() {
        return values;
    }

    ~BaseADC() {
        delete coefficients;
        delete values;
    }
};
}

#endif /* BASE_ADC_H_ */