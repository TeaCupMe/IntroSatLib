/*
 * Morse.cpp
 *
 *  Created on: Dec 1, 2025
 *      Author: samsa
 */

#ifdef ARDUINO_AVR_ATmega328

#include "IRTransceiver.h"
#include "IRremote.hpp"


namespace IntroSatLib {

ISL_StatusTypeDef IRTransceiver::Init(
    uint8_t receive,
    uint8_t transmit,
    ProtocolTimings timings) {

    _timings = timings;
    _receive = receive;
    _transmit = transmit;

    IrSender.begin(_transmit);
    IrReceiver.begin(_receive);
}

ISL_StatusTypeDef IRTransceiver::Init(
    uint8_t receive,
    uint8_t transmit) {
    Init(receive, transmit, ProtocolTimings());
}

ISL_StatusTypeDef IRTransceiver::Init(ProtocolTimings timings) {
    Init(RX_PIN_DEFAULT, TX_PIN_DEFAULT, timings);
}

ISL_StatusTypeDef IRTransceiver::Init() {
    Init(RX_PIN_DEFAULT, TX_PIN_DEFAULT, ProtocolTimings());
}


ISL_StatusTypeDef IRTransceiver::decodeProtocol(const uint8_t* rawData, uint8_t rawLength, uint8_t* result) {
    uint32_t markDurationMicros;
    uint32_t spaceDurationMicros;
    
    for (uint16_t i = 1; i < rawLength; i += 2) {
        markDurationMicros = (uint32_t)rawData[i] * (uint32_t)MICROS_PER_TICK;      // TODO: fix it
        spaceDurationMicros = (uint32_t)rawData[i+1] * (uint32_t)MICROS_PER_TICK;   // TODO: fix it
        
        if (abs((int32_t)markDurationMicros - (int32_t)_timings._dashMark) < (int32_t)_timings._errorScale) {
            result[i/2] = 1;
        } else if (abs((int32_t)markDurationMicros - (int32_t)_timings._dotMark) < (int32_t)_timings._errorScale) {
            result[i/2] = 0;
        } else {
            // result[i/2] = 255;
            return ISL_ERROR;
        }
    }

    return ISL_OK;
}


ISL_StatusTypeDef IRTransceiver::transmit(uint8_t* tx_buffer, uint16_t length) {

    if (length > 32) return ISL_ERROR;

    uint16_t rawData[64];
    int index = 0;

    for (int i = 0; i < length; i++) {
        if (tx_buffer[i] == 1) {
            rawData[index++] = _timings._dashMark;
            rawData[index++] = _timings._dashSpace;
        } else {
            rawData[index++] = _timings._dotMark;
            rawData[index++] = _timings._dotSpace;
        }
    }

    IrSender.sendRaw(rawData, length * 2, 38); // Отправляем на 38 кГц
    return ISL_OK;
}



ISL_StatusTypeDef IRTransceiver::receive(uint8_t* rx_buffer, uint16_t* length) {
    if (IrReceiver.available()) {
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            uint8_t pre_buffer[32];
            if (decodeProtocol(IrReceiver.irparams.rawbuf,
                            IrReceiver.irparams.rawlen,
                            pre_buffer) == ISL_ERROR) {
                IrReceiver.resume();
                return ISL_ERROR;
            }
            
            memcpy(rx_buffer, pre_buffer, sizeof(pre_buffer));
            *length = (uint16_t)IrReceiver.irparams.rawlen / 2;
        } else {
            IrReceiver.resume();
            return ISL_ERROR;
        }
        
        IrReceiver.resume();
        return ISL_OK;
    }
    return ISL_ERROR;
}

} /* namespace IntroSatLib */

#endif