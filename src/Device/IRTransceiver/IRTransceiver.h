#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"
#include "Adapter/GPIO.h"


namespace IntroSatLib {

class IRTransceiver
{
public:

protected:

private:

    static constexpr uint16_t defaultTimeout = 100;
 
    ISLIRReceiver receiver;
    ISLIRTransmitter transmitter;

public:

    IRTransceiver(ISLIRReceiver _receiver, 
                  ISLIRTransmitter _transmitter): 
                                                receiver(_receiver), 
                                                transmitter(_transmitter)  { };

    ISL_StatusTypeDef TransmitIR(uint8_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef ReceiveIR(uint8_t* buff, uint8_t nbytes, uint16_t timeout=defaultTimeout);

};


}

#endif