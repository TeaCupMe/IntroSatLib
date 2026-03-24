#ifndef STEPPER_28BYJ_H_
#include "Device/Base/BaseStepperDirect.h"

namespace IntroSatLib {

class Stepper28BYJ: public BaseStepperDirect<4> {
    const uint16_t stepsPerRevolution28BYJ = 2038;
public:
    Stepper28BYJ(): BaseStepperDirect(stepsPerRevolution28BYJ, false) {};
};

};


#endif