#ifndef ISLIRPROTOCOL_H_
#define ISLIRPROTOCOL_H_

#include <stdint.h>

namespace IntroSatLib
{
    namespace IRProtocol
    {        
        struct ProtocolTimings {
            uint16_t markStart = 30;
            uint16_t spaceStart = 10;
            uint16_t mark0 = 10;
            uint16_t mark1 = 20;
            uint16_t space0 = 10;
            uint16_t space1 = 10;
            uint16_t errorScale = 5;
            uint16_t maxSpaceWidth = 30;
            uint16_t maxMarkWidth = 30;
        };
    }
}

#endif