#ifndef ADS7830_H_
#define AD7830_H_
#include "Device/Base/BaseADC.h"
#include "Device/I2CDevice.h"
#include "Adapter/System.h"
#include "stdint.h"

namespace IntroSatLib {

class ADS7830: public BaseADC, protected I2CDevice {
public:
    /**
     * В режиме работы Differential могут быть выбраны только 4 пары каналов:
     *      CH0 CH1
     *      CH2 CH3
     *      CH4 CH5
     *      CH6 CH7
     * При выборе канала CHANNEL задаётся, какой канал соответствует IN+
     */
    enum Mode: uint8_t {
        DIFFERENTIAL = 0,
        SINGLE_ENDED
    };

    enum Channel: uint8_t {
        CH0 = 0, // CH0 single-ended or IN+ in differential 
        CH2,
        CH4,
        CH6,
        CH1,
        CH3,
        CH5,
        CH7
    };

    enum ReferenceMode: uint8_t {
        REF_EXTERNAL = 0,
        REF_INTERNAL
    };

    enum AdcOn : uint8_t {
        ADC_OFF = 0,
        ADC_ON
    };

private:
    static const uint8_t BASE_ADDRESS = 0x48;

    uint8_t _cmd = 0x0;
    uint8_t _resolution = 8;
    float _coeffConvert; // to convert to volts with bit-depth 8

//    ISL_StatusTypeDef SendCommand(Mode mode, Channel channel, ReferenceMode ref, AdcOn on);

public:
    ADS7830(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

    ISL_StatusTypeDef Init();
    ISL_StatusTypeDef Init(float uRef);

    void SetMode(Mode mode);
    void SetChannel(Channel channel);
    void SetAdcOn(AdcOn on);

    ISL_StatusTypeDef SetRefInternal();
    ISL_StatusTypeDef SetRefExternal(float uRef);
    ISL_StatusTypeDef PowerDown();
    
    uint8_t GetRawValue(Channel channel, Mode mode = Mode::SINGLE_ENDED);
    
    float GetValue(Channel channel, Mode mode = Mode::SINGLE_ENDED); 
    float GetValue(float coeff, Channel channel, Mode mode = Mode::SINGLE_ENDED); 
};

}

#endif