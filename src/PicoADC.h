#ifndef PICO_ADC_H_
#define PICO_ADC_H_

#include "Adapter/I2C.h"
#include "Device/ADS7830/ADS7830.h"

namespace IntroSatLib {

class PicoADC : public ADS7830 {
public:
    enum Channel: uint8_t {
        CH_VIN = ADS7830::Channel::CH0,
        CH_VBAT = ADS7830::Channel::CH1,
        CH_VOUT = ADS7830::Channel::CH2,
        CH_CIN = ADS7830::Channel::CH3,
        CH_COUT = ADS7830::Channel::CH4,
    };

    PicoADC(interfaces::I2C i2c): ADS7830(i2c) {}

    ISL_StatusTypeDef Init() {
        RETURN_STATUS_IF_NOT_OK_SILENT(ADS7830::Init());
        InitChannel(CH_VIN, 4.0f);
        InitChannel(CH_VBAT, 1/0.585f);
        InitChannel(CH_VOUT, 1/0.585f);
        InitChannel(CH_CIN, 1000/6.2f);
        InitChannel(CH_COUT, 1000/6.2f);

        return ISL_StatusTypeDef::ISL_OK;
    }

    float GetValue(Channel channel) {
        return ADS7830::GetValue((ADS7830::Channel) channel);
    }

    float GetVIN() {
        return GetValue(Channel::CH_VIN);
    }

    float GetVBAT() {
        return GetValue(Channel::CH_VBAT);
    }

    float GetVOUT() {
        return GetValue(Channel::CH_VOUT);
    }

    float GetCIN() {
        return GetValue(Channel::CH_CIN);
    }

    float GetCOUT() {
        return GetValue(Channel::CH_COUT);
    }
};

}

#endif /* PICO_ADC_H_ */