#include "IntroSatLib.h"
#include "Stepper28BYJ.h"

using namespace IntroSatLib;

Stepper28BYJ stepper;

char rxBuf[20];


void setup() {
    Serial.begin(115200, SERIAL_8E1);
    stepper.SetCoilPin(0, 2);
    stepper.SetCoilPin(1, 3);
    stepper.SetCoilPin(2, 4);
    stepper.SetCoilPin(3, 5);

    stepper.Init(IntroSatLib::BaseStepperDirect<4>::FULLSTEP);
}

void loop() {
    if (!Serial.available()) delay(1);

    switch (Serial.read()) {
        case 'r':
            stepper.StepCW(Serial.parseInt());
            break;
        case 'l':
            stepper.StepCCW(Serial.parseInt());
            break;
        case 'i':
            stepper.RotateCW(Serial.parseFloat());
            break;
        case 'o':
            stepper.RotateCCW(Serial.parseFloat());
            break;
        case 'p':
            Serial.printf("Stepper position is %d, %f degrees", stepper.GetPosition(), stepper.GetAngle());
            break;
    }
}