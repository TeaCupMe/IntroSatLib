/*
 * Morse.cpp
 *
 *  Created on: Dec 1, 2025
 *      Author: samsa
 */

#ifdef ARDUINO_AVR_ATmega328

#include "Morse.h"

namespace IntroSatLib {


ISL_StatusTypeDef Morse::Init(
    uint8_t receive,
    uint8_t transmit,
    ProtocolTimings timings) {
    return IRTransceiver::Init(receive, transmit, timings);
}

ISL_StatusTypeDef Morse::Init(
    uint8_t receive,
    uint8_t transmit) {
    return IRTransceiver::Init(receive, transmit);
}

ISL_StatusTypeDef Morse::Init(ProtocolTimings timings) {
    return IRTransceiver::Init(timings);
}

ISL_StatusTypeDef Morse::Init() {
    return IRTransceiver::Init();
}


ISL_StatusTypeDef Morse::transmit(uint8_t* tx_buffer, int length) {
    if (length > 32) return ISL_ERROR;

    uint8_t data[32];
    for(uint8_t i = 0; i < length; i++) {
        switch (tx_buffer[i])
        {
        case '.':
            data[i] = 0;
            break;
        case '_':
            data[i] = 1;
            break;
        default:
            return ISL_ERROR;
        }
    }
    return IRTransceiver::transmit(data, length);
}


ISL_StatusTypeDef Morse::receive(uint8_t* rx_buffer, uint16_t* length) {
    uint8_t data[32];
    RETURN_STATUS_IF_NOT_OK_SILENT(IRTransceiver::receive(data, length)); //TODO: think about lehgth

    bool flag = false;
    for (uint8_t i = 0; i < length; i++) {
        if (flag) break;
        switch (data[i])
        {
        case 0:
            rx_buffer[i] = '.';
            break;
        case 1:
            rx_buffer[i] = '_';
            break;
        default:
            flag = true;
            break;
        }
    }

    return ISL_OK;
}



} /* namespace IntroSatLib */

#endif