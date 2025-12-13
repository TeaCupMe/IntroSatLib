/*
 * BaseTransceiver.h
 *
 *  Created on: Dec 1, 2025
 *      Author: samsa
 */

#ifndef BASE_TRANSCEIVER_H_
#define BASE_TRANSCEIVER_H_

#include "Device/Device.h"
#include "IntroSatLib_def.h"

namespace IntroSatLib {

class BaseTransceiver {
protected:

public:
	virtual ISL_StatusTypeDef transmit(uint8_t* tx_buffer, uint16_t length);
    virtual ISL_StatusTypeDef receive(uint8_t* rx_buffer, uint16_t* length);
};

} /* namespace IntroStratLib */


#endif /* BASE_TRANSCEIVER_H_ */