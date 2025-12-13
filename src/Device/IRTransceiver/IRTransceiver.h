/*
 * IRTransCeiver.h
 *
 *  Created on: Dec 1, 2025
 *      Author: samsa
 */

#ifndef IR_TRANSCEIVER_H_
#define IR_TRANSCEIVER_H_

#ifndef ARDUINO_AVR_ATmega328
    #error "This not yet supported"
#else

#include "Device/Base/BaseTransceiver.h"
// #include "Adapter/GPIO.h"

#define NO_DECODER
// #include "IRremote.hpp"



namespace IntroSatLib {

#define RX_PIN_DEFAULT 2 // Пин, к которому подключён ИК-приёмник
#define TX_PIN_DEFAULT 3 // Пин, к которому подключён ИК-передатчик


#define BIT_1_HIGH 15000 // Длительность mark для "1"
#define BIT_1_LOW 5000   // Длительность space для "1"
#define BIT_0_HIGH 5000  // Длительность mark для "0"
#define BIT_0_LOW 5000   // Длительность space для "0"
#define ERROR_SCALE 3000


class IRTransceiver: public BaseTransceiver, protected Device {
public:

    struct ProtocolTimings {
        uint16_t _dashMark = BIT_1_HIGH;
        uint16_t _dashSpace = BIT_1_LOW;
        uint16_t _dotMark = BIT_0_HIGH;
        uint16_t _dotSpace = BIT_0_LOW;
        uint16_t _errorScale = ERROR_SCALE;
    };

private:

    ProtocolTimings _timings = ProtocolTimings();
    uint8_t _receive = -1;
    uint8_t _transmit = -1;

    ISL_StatusTypeDef decodeProtocol(const uint8_t* rawData, 
                                    uint8_t rawLength, 
                                    uint8_t* result);

    // ISL_StatusTypeDef encodeProtocol(const uint8_t* rawData, 
    //                                 uint8_t rawLength, 
    //                                 uint8_t* result);

public:

    ISL_StatusTypeDef Init() override;
    ISL_StatusTypeDef Init(ProtocolTimings timings);
    ISL_StatusTypeDef Init(
        uint8_t receive,
        uint8_t transmit
    );
    ISL_StatusTypeDef Init(
        uint8_t receive,
        uint8_t transmit,
        ProtocolTimings timings
    );

    // ISL_StatusTypeDef Enable() override;
    // ISL_StatusTypeDef Disable() override;

	ISL_StatusTypeDef transmit(uint8_t* tx_buffer, uint16_t length) override;
    ISL_StatusTypeDef receive(uint8_t* rx_buffer, uint16_t* length) override;
};

} /* namespace IntroStratLib */
#endif

#endif /* IR_TRANSCEIVER_H_ */