#ifndef BASEIRDECODER_H_
#define BASEIRDECODER_H_

#include "IntroSatLib_def.h"

namespace IntroSatLib
{

class BaseIRDecoder
{
public:
    virtual ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength) = 0;
};

}

#endif