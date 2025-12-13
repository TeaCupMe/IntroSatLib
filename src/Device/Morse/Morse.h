/*
 * Morse.h
 *
 *  Created on: Dec 1, 2025
 *      Author: samsa
 */


#ifndef MORSE_H_
#define MORSE_H_

#ifndef ARDUINO_AVR_ATmega328
    #error "This not yet supported"
#else

#include "Device/IRTransceiver/IRTransceiver.h"

namespace IntroSatLib {

class Morse: private IRTransceiver {
private:
    
public:
    Morse() {};

    ISL_StatusTypeDef Init();
    ISL_StatusTypeDef Init(ProtocolTimings timings);
    ISL_StatusTypeDef Init(
        uint8_t receive,
        uint8_t transmit
    );
    ISL_StatusTypeDef Morse::Init(
        uint8_t receive,
        uint8_t transmit,
        ProtocolTimings timings);

    // ISL_StatusTypeDef Enable() override;
    // ISL_StatusTypeDef Disable() everride;

    ISL_StatusTypeDef transmit(uint8_t* tx_buffer, int length);
    ISL_StatusTypeDef receive(uint8_t* rx_buffer, uint16_t* length);


    ~Morse() {};
};


} /* namespace IntroSatLib */

#endif

#endif /* MORSE_H_ */