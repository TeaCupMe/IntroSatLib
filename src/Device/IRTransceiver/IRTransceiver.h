#ifndef IRTRANSCEIVER_H_
#define IRTRANSCEIVER_H_

#if defined(ARDUINO)

#include "Device/ISLIRReceiver/ISLIRReceiver.h"
#include "Device/ISLIRTransmitter/ISLIRTransmitter.h"


namespace IntroSatLib {

class IRTransceiver : Device
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

    ISL_StatusTypeDef Init() override;

    ISL_StatusTypeDef TransmitIR(uint8_t* txBuff, uint16_t length);
    
    void ProcessReceivingIR();
    bool Available();
    ISL_StatusTypeDef GetRawData(uint16_t* buff, uint16_t length);
    ISL_StatusTypeDef ISLDecode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength);
    ISL_StatusTypeDef GetMessage(uint8_t* rxbuff, uint16_t rxLength);

};


}

#endif
#endif