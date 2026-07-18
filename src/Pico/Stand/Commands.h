#ifndef PICO_STAND_COMMANDS_H_
#define PICO_STAND_COMMANDS_H_

#include <stdint.h>

namespace IntroSatLib::Pico::Stand {

enum class Commands : uint8_t {
    SET_ANGLE = 0x01,       // Встать на угол
    GET_ANGLE = 0x02,       // Запрос угла
    STEP_CW = 0x03,         // Сделать шаги по ЧС
    STEP_CCW = 0x04,        // Сделать шаги против ЧС
    SET_STEP = 0x05,        // Встать в положение
    ROTATE_REL_CW = 0x06,   // Поворот на угол по ЧС
    ROTATE_REL_CCW = 0x07   // Поворот на угол против ЧС
};

}

#endif /* PICO_STAND_COMMANDS_H_ */