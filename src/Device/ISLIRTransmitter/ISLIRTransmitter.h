#ifndef ISLIRTRANSMITTER_H_
#define ISLIRTRANSMITTER_H_

#include "Device/Device.h"
#include "Adapter/GPIO.h"


namespace IntroSatLib
{

class ISLIRTransmitter : Device
{

public:

protected:

private:
    interfaces::GPIO rxPin;

public:

    ISLIRTransmitter(interfaces::GPIO _rxPin) : rxPin(_rxPin) { }
    ISL_StatusTypeDef Init() { }

};

}


#endif