#include "PicoStand.h"

IntroSatLib::PicoStand::PicoStand(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
    
}

ISL_StatusTypeDef IntroSatLib::PicoStand::Init()
{
    return IsReady();
}

float IntroSatLib::PicoStand::GetAngle()
{
    float angle = 0;
    ReadRegisterI2C(CMD_GET_ANGLE, (uint8_t*)&angle, 4);
    return angle;
}

ISL_StatusTypeDef IntroSatLib::PicoStand::StepCW(int16_t steps)
{
    return SetRegisterI2C(CMD_STEP_CW, (uint8_t*)&steps, 2);
}

ISL_StatusTypeDef IntroSatLib::PicoStand::StepCCW(int16_t steps)
{
    return SetRegisterI2C(CMD_STEP_CCW, (uint8_t*)&steps, 2);
}

ISL_StatusTypeDef IntroSatLib::PicoStand::SetAngle(float angle)
{
    return SetRegisterI2C(CMD_SET_ANGLE, (uint8_t*)&angle, 4);
}
