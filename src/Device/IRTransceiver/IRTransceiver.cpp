#define ISL_INTERNAL

#include "Adapter/GPIO.h"
#if defined(ISL_GPIO_ENABLED)

#include "Device/IRTransceiver/IRTransceiver.h"
#include "Adapter/System.h"

namespace IntroSatLib
{

    ISL_StatusTypeDef IRTransceiver::getRawData(uint16_t* buff, uint8_t nbytes, uint16_t timeout)
    {
        uint32_t endTime;
        pins.receiver.waitReset(timeout);
        uint32_t startTime = system::GetTick();
        while(pins.receiver.read() != 1)
		{
			if ((system::GetTick() - startTime) > timeout) { return ISL_ERROR; }
		}
        endTime = system::GetTick();

        buff[0] = endTime - startTime;

        return ISL_OK;
    }


}

#endif
