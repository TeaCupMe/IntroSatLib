#include "FSC-BT986.h"

namespace IntroSatLib {

    uint16_t FSC_BT986::executeATCommand(uint8_t* command, uint8_t* rxbuffer, uint16_t length) {
        uint8_t buff;
        while(_uart->available()) _uart->receive(&buff, 1); // Clear input buffer

        _uart->transmit(command, strlen((const char*)command));
        
        uint16_t itter = 0;
        timer = system::GetTick();

        while (itter < length || system::GetTick() - timer <= timeout) {
            if (_uart->available()) {
                _uart->receive(rxbuffer + itter, 1);
                itter++;
            }
        }

        return itter;
    }

}