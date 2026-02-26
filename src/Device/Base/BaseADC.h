#ifndef BASE_ADC_H_
#define BASE_ADC_H_

#include "Device/Device.h"
#include "stddef.h"

namespace IntroSatLib {

template <size_t ChannelCount>
class BaseADC: public Device {

private:
    const uint8_t resolution;
    float coefficients[ChannelCount];
public:
	virtual float GetValue(uint8_t channel) = 0;
};
}

#endif /* BASE_ADC_H_ */