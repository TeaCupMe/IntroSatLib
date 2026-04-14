#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#include "Adapter/GPIO.h"


namespace IntroSatLib {

class IRTransceiver {
public:


    struct IRPins
    {
        interfaces::GPIO transmitter = interfaces::GPIO();
        interfaces::GPIO receiver = interfaces::GPIO();
    };


protected:

private:

    static constexpr uint16_t defaultTimeout = 100;
    static constexpr uint16_t timingFor1 = 1500;
    static constexpr uint16_t timingFor0 = 500;
    static constexpr uint16_t startTiming = 3000;
 
    IRPins pins;

    // ISL_StatusTypeDef getRawData(uint8_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);

public:

    IRTransceiver(IRPins _pins): pins(_pins) { };

    ISL_StatusTypeDef transmit(uint8_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef receive(uint8_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef getRawData(uint16_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);

};


}

#endif