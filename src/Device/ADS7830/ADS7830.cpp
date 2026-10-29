#define ISL_INTERNAL

#include "Adapter/I2C.h"

#ifdef ISL_I2C_ENABLED

#include "ADS7830.h"
#include "Adapter/I2C.h"
#include "Device/I2CDevice.h"
#include "stdint.h"

namespace IntroSatLib {
IntroSatLib::ADS7830::ADS7830(interfaces::I2C i2c, uint8_t address): I2CDevice(i2c, address), BaseADC(channelCount) {
}

ISL_StatusTypeDef ADS7830::RequestConversion(Channel channel = Channel::CH0) {
    uint8_t cmd = 0x0;
    cmd |= channel << 4;
    cmd |= refMode << 3;
    cmd |= adcOn << 2;
    return WriteI2C(cmd); 
}

ISL_StatusTypeDef ADS7830::Init() {
    return UseInternalReference();
}

ISL_StatusTypeDef ADS7830::Init(float uRef) {
    return UseExternalReference(uRef);
}

void ADS7830::SetAdcOn(AdcMode on) {
    adcOn = on;
}

ISL_StatusTypeDef ADS7830::UseExternalReference(float refVoltage)
{
    refMode = ReferenceMode::REF_EXTERNAL;
    uRef = refVoltage;
    return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef ADS7830::UseInternalReference()
{
    refMode = ReferenceMode::REF_INTERNAL;
    uRef = 2.5f;
    return ISL_StatusTypeDef::ISL_OK;    
}

ISL_StatusTypeDef ADS7830::PowerDown() {
    adcOn = AdcMode::ADC_OFF;
    return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef ADS7830::PollChannel(uint8_t channel) {
    uint8_t buf = 0;
    if (channel >= channelCount) return ISL_StatusTypeDef::ISL_ERROR;

    if (RequestConversion(static_cast<Channel>(channel)) != ISL_OK) return ISL_StatusTypeDef::ISL_ERROR;
        
    if (ReadI2C(&buf) != ISL_OK) buf = 0;
    
    values[channel] = buf;
    return ISL_StatusTypeDef::ISL_OK;
}

}

#endif /* ISL_I2C_ENABLED */

