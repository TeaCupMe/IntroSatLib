#ifndef ISLIRDECODER_H_
#define ISLIRDECODER_H_

#include "Device/Base/BaseIRDecoder.h"

namespace IntroSatLib
{

class ISLIRDecoder : public BaseIRDecoder
{
public:

    struct ProtocolTimings {
        uint16_t markStart = 30;
        uint16_t spaceStart = 10;
        uint16_t mark0 = 10;
        uint16_t space0 = 10;
        uint16_t mark1 = 20;
        uint16_t space1 = 10;
        uint16_t markEnd = 30;
        uint16_t errorScale = 5;
    };

private:

    ProtocolTimings timings;

public:

    ISLIRDecoder() = default;
    ISLIRDecoder(const ProtocolTimings& _timings) : timings(_timings) { }

    ISL_StatusTypeDef Decode(uint16_t* rawData, uint16_t rawLength, uint8_t* rxbuff, uint16_t rxLength) override;

};

}

#endif