#ifndef DEVICE_SPIDEVICE_H_
#define DEVICE_SPIDEVICE_H_

#include "Adapter/SPI.h"
#include "Adapter/GPIO.h"
#include "Device/Device.h"
#include "IntroSatLib_def.h"

namespace IntroSatLib {

class SPIDevice: public Device {
private:
    interfaces::SPI spi;
    bool useCs = false;
    interfaces::GPIO cs;
    bool csActiveLow = true;

public:
    void Select();
    void Deselect();

    SPIDevice(interfaces::SPI _spi);
    virtual ISL_StatusTypeDef Init() override;
    
    ISL_StatusTypeDef TransmitSPI(uint8_t* out, uint8_t len);
    ISL_StatusTypeDef ReceiveSPI(uint8_t* in, uint8_t len);
    ISL_StatusTypeDef TransmitReceiveSPI(uint8_t* out, uint8_t* in, uint8_t len);

    void SetCS(interfaces::GPIO _cs, bool _csActiveLow = true);

};

};

#endif /* DEVICE_SPIDEVICE_H_ */
