#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#if defined(ARDUINO_AVR_ATmega328)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"


namespace IntroSatLib {

class IRTransceiver : Device
{
public:

protected:

private:
 
    ISLIRReceiver receiver;
    ISLIRTransmitter transmitter;

public:

    IRTransceiver(ISLIRReceiver _receiver, 
                  ISLIRTransmitter _transmitter): 
                                                receiver(_receiver), 
                                                transmitter(_transmitter)  { };

    ISL_StatusTypeDef Init() override;

    ISL_StatusTypeDef TransmitIR(uint8_t* txBuff, uint16_t length);
    
    void ProcessReceivingIR();
    uint16_t Available();
    ISL_StatusTypeDef GetData(uint8_t* buff, uint16_t length);

};


}

#endif
#endif