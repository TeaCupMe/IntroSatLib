#define ISL_INTERNAL

#include "Adapter/I2C.h"

#ifdef ISL_I2C_ENABLED

#include "ADS7830.h"
#include "Adapter/I2C.h"
#include "Device/I2CDevice.h"

namespace IntroSatLib {
IntroSatLib::ADS7830::ADS7830(interfaces::I2C i2c, uint8_t address) : 
        I2CDevice(i2c, address) {
}

ISL_StatusTypeDef ADS7830::SendCommand(Channel channel = Channel::CH0, 
                                       ReferenceMode ref = ReferenceMode::INTERNAL,
                                       AdcOn on = AdcOn::ADC_OFF) {
    uint8_t cmd = 0x0;
    cmd |= mode << 7;
    cmd |= channel << 4;
    cmd |= ref << 3;
    cmd |= on << 2;
    return WriteI2C(cmd); 
}

ISL_StatusTypeDef ADS7830::Init() {
    return SetRefInternal();
}

ISL_StatusTypeDef ADS7830::Init(float uRef) {
    return SetRefExternal(uRef);
}

ISL_StatusTypeDef ADS7830::SetRefInternal() {
    float uRef = 2.5f;
    _coeffConvert = uRef / (1 << _resolution);
    SetAdcOn(ADC_OFF);
    _cmd &= 0b11110111;
    _cmd |= ReferenceMode::REF_INTERNAL << 3;
    ISL_StatusTypeDef status = WriteI2C(_cmd);
    system::Delay(1); // depends on capacitor on REF_IN / REF_OUT pin
    return status; 
}

ISL_StatusTypeDef ADS7830::SetRefExternal(float uRef) {
    _coeffConvert = uRef / (1 << _resolution);
    SetAdcOn(ADC_OFF);
    _cmd &= 0b11110111;
    _cmd |= ReferenceMode::REF_EXTERNAL << 3;
    ISL_StatusTypeDef status = WriteI2C(_cmd);
    system::Delay(1); // depends on capacitor on REF_IN / REF_OUT pin
    return status; 
}

void ADS7830::SetAdcOn(AdcOn on) {
//    uint8_t cmd = 0x0;
    _cmd &= 0b11111011;
    _cmd |= on << 2;
//   return WriteI2C(cmd); 
}

ISL_StatusTypeDef ADS7830::PowerDown() {
    _cmd &= 0b11110000;
    return WriteI2C(_cmd);
}

uint8_t ADS7830::GetRawValue(Channel channel) {
    ISL_StatusTypeDef status;
    uint8_t buf;

    SetAdcOn(AdcOn::ADC_ON);
    SetMode(mode);
    SetChannel(channel);

    status = WriteI2C(_cmd);
    if (status != ISL_OK)    
        return -1;
        
    status = ReadI2C(&buf);
    if (status != ISL_OK)    
        return -1;
        
    return buf;
}

    
float ADS7830::GetValue(Channel channel) {
    uint8_t buf;
    buf = GetRawValue(channel, mode);
    return ((float)buf) * _coeffConvert;
}

float ADS7830::GetValue(float coeff, Channel channel) {
    return coeff * GetValue(channel, mode);
}

}

#endif /* ISL_I2C_ENABLED */

