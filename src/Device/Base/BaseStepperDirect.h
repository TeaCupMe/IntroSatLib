#ifndef BASE_STEPPERDIRECT_H_
#define BASE_STEPPERDIRECT_H_
#include "Device/Base/BaseStepper.h"
#include "Adapter/GPIO.h"

namespace IntroSatLib {

template <uint8_t CoilsCount = 4>
class BaseStepperDirect: protected BaseStepper {
public:
    enum StepMode {
        FULLSTEP,
        HALFSTEP,
        // MICROSTEP
    };
private:
    interfaces::GPIO* coils[CoilsCount];
    bool _activeHigh;
    StepMode _stepMode;

    uint8_t currentPhase = 0;
    uint8_t phases = 0;

    void setCoils() {
        for (uint8_t coil = 0; coil < CoilsCount; coil++) {
            if (coil == currentPhase / _stepMode) coils[coil]->write(_activeHigh);
            //        if in halfstep mode AND we're on odd step AND coil is to be activated on next step 
            else if (_stepMode == HALFSTEP &&  currentPhase % 2  && coil == (((currentPhase + 1) % (phases)) / 2)) coils[coil]->write(_activeHigh);
            else coils[coil]->write(!_activeHigh);
        }
    }

    void _stepCW(uint16_t steps = 1) override {
        for (uint16_t i = 0; i < steps; i++) {
            currentPhase = (currentPhase + 1) % (phases);
            setCoils();
        }
    }

    void _stepCCW(uint16_t steps = 1) override {
        for (uint16_t i = 0; i < steps; i++) {
            currentPhase = currentPhase > 0 ? currentPhase - 1 : phases - 1;
            setCoils();
        }
    }

public: 
    BaseStepperDirect(uint16_t stepsPerRevolution, bool activeHigh = true) {
        this->_stepsPerRevolution = stepsPerRevolution;
        _activeHigh = activeHigh;
    }

    ISL_StatusTypeDef Init(StepMode stepMode) {
        _stepMode = stepMode;
        phases = CoilsCount * stepMode;
        return ISL_OK;
    }
    ISL_StatusTypeDef Init() override {
        return Init(StepMode::FULLSTEP);
    }

    void SetCoilPin(uint8_t coil, interfaces::GPIO_HANDLE_TYPE port, uint16_t pin = 0) {
        if (coil < CoilsCount) {
            coils[coil] = new interfaces::GPIO(&port, pin);
        }
    }
};

} /* namespace IntroSatLib */

#endif /* BASE_STEPPERDIRECT_H_ */