#ifndef BASE_STEPPER_H_
#define BASE_STEPPER_H_
#include "stdint.h"
#include "Device/Device.h"
#include "math.h"


namespace IntroSatLib {


class BaseStepper: Device {

protected:
    
    uint16_t _stepsPerRevolution;

    uint16_t position = 0;

    bool noZeroCrossing = false;

    virtual void _stepCW(uint16_t steps = 1) = 0;
    virtual void _stepCCW(uint16_t steps = 1) = 0;


public:
    virtual ISL_StatusTypeDef Init() override = 0;
    
    void StepCW(uint16_t steps = 1) {
        if (noZeroCrossing && position + steps >= _stepsPerRevolution) {
            steps = _stepsPerRevolution - position;
        }
        _stepCW(steps);
        position = (position + (steps % _stepsPerRevolution)) % _stepsPerRevolution;
    }
    void StepCCW(uint16_t steps = 1) {
        if (noZeroCrossing && position < steps) {
            steps = position;
        }
        
        _stepCCW(steps);
        // Should kinda work, but need requires to be checked for 1-off error                            VVVVVVVVVVVVVVVVVVVVVV   HERE   VVVVVVVVVVVVVVVVVVV
        position = position > (steps % _stepsPerRevolution) ? position - (steps % _stepsPerRevolution) : _stepsPerRevolution - (steps % _stepsPerRevolution);
    }

    uint16_t GetPosition() {
        return position;
    }

    void SetPosition(uint16_t newPosition) {
        position = newPosition;
    }

    void RotateCW(float angle) {
        StepCW((uint16_t) (angle * _stepsPerRevolution) / 360);
    }

    void RotateCCW(float angle) {
        StepCCW((uint16_t) (angle * _stepsPerRevolution) / 360);
    }

    void Rotate(float angle) {
        if (angle > 0) {
            RotateCW(angle);
        } else {
            RotateCCW(-angle);
        }
    }

    void RotateTo(float targetAngle) {
        float diff = targetAngle - GetAngle();
        if (!noZeroCrossing) {
            if      (diff >  _stepsPerRevolution / 2)  diff -= _stepsPerRevolution;
            else if (diff < -_stepsPerRevolution / 2) diff += _stepsPerRevolution;
        }
        Rotate(diff);
    }

    float GetAngle() {
        return (position * 1.0) * 360 / _stepsPerRevolution;
    }

    void enableZeroCrossing() {
        noZeroCrossing = false;
    }
    void disableZeroCrossing() {
        noZeroCrossing = true;
    }

};

}; /* namespace IntroSatLib */

#endif /* BASE_STEPPER_H_ */