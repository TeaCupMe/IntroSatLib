#ifndef PICOSTAND_H_
#define PICOSTAND_H_
#include "Adapter/I2C.h"
#include "Device/I2CDevice.h"

namespace IntroSatLib {

class PicoStand: private I2CDevice {
    
    private:
	static const uint8_t BASE_ADDRESS = 0x0C;
    enum Command {
        CMD_SET_ANGLE = 0x01,       // Встать на угол
        CMD_GET_ANGLE = 0x02,       // Запрос угла
        CMD_STEP_CW = 0x03,         // Сделать шаги по ЧС
        CMD_STEP_CCW = 0x04,        // Сделать шаги против ЧС
    };

public:
    PicoStand(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);
    ISL_StatusTypeDef Init();

    float GetAngle();

    ISL_StatusTypeDef StepCW(int16_t steps);
    ISL_StatusTypeDef StepCCW(int16_t steps);
    ISL_StatusTypeDef SetAngle(float angle);

};


};



#endif