#ifndef PICO_STAND_H_
#define PICO_STAND_H_

#include "Adapter/I2C.h"
#include "Device/I2CDevice.h"
#include "Pico/Stand/Commands.h"

namespace IntroSatLib::Pico {
using namespace IntroSatLib::Pico::Stand;
/**
 * \~russian @brief Класс для работы с поворотным стендом IntroSat.Pico.
 * \~english @brief Class for interfacing with the IntroSat.Pico stand.
 */
class Stand : private I2CDevice {
    static const uint8_t I2C_ADDRESS = 0x25; ///< Адрес стенда на шине I2C

public:

    Stand(interfaces::I2C i2c) : I2CDevice(i2c, I2C_ADDRESS) 
    {}

    ISL_StatusTypeDef SetAngle(float angle) {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::SET_ANGLE)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&angle), sizeof(angle));
    }

    float GetAngle() {
        float angle{0.0f};
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::GET_ANGLE)));
        if (!ReadI2C(reinterpret_cast<uint8_t*>(&angle), sizeof(angle))) {
            return 0.0f;
        }
        return angle;
    }

    ISL_StatusTypeDef StepCW(uint16_t steps)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::STEP_CW)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&steps), sizeof(steps));
    }

    ISL_StatusTypeDef StepCCW(uint16_t steps)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::STEP_CCW)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&steps), sizeof(steps));
    }

    ISL_StatusTypeDef SetStep(uint16_t step)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::SET_STEP)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&step), sizeof(step));
    }

    ISL_StatusTypeDef RotateCW(float angle)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::ROTATE_REL_CW)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&angle), sizeof(angle));
    }

    ISL_StatusTypeDef RotateCCW(float angle)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(static_cast<uint8_t>(commands::Commands::ROTATE_REL_CCW)));
        return WriteI2C(reinterpret_cast<uint8_t*>(&angle), sizeof(angle));
    }
};

}

#endif /* PICO_STAND_H_ */