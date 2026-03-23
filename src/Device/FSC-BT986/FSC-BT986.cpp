#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC-BT986.h"

namespace IntroSatLib {

    ISL_StatusTypeDef FSC_BT986::Init() {

        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("PIOCFG",
                    (pins.mode.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0",
                    (pins.disconnect.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0"
                ));


        setHardMode(1);
        setPrgMode(1);

        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("BTEN", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("LPM", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("FLOWCTL", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SSP", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("MODE", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SECURITY", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SPPCFG", (uint8_t*)"0"));

        setHardMode(0);
        setPrgMode(0);
        
        return ISL_OK;
    }
    
    ISL_StatusTypeDef FSC_BT986::writePIN(uint8_t* pinCode, uint16_t timeout) {
        return writeAT("PIN", timeout, pinCode);
    }

    ISL_StatusTypeDef FSC_BT986::readPIN(uint8_t* rxbuff, uint16_t timeout) {
        return readAT("PIN", rxbuff, 30, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::readMAC(uint8_t* rxbuff, uint16_t timeout) {
        return readAT("ADDR", rxbuff, 28, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::connect(uint8_t* mac, uint16_t timeout) {
        return writeAT("SPPCONN", timeout, mac);
    }

    ISL_StatusTypeDef FSC_BT986::reboot(uint16_t timeout) {
        return writeAT("REBOOT", timeout);
    }

    ISL_StatusTypeDef FSC_BT986::setPrgMode(uint8_t state, uint16_t timeout) {
        return writeAT("TPMODE", timeout, (state == 0) ? (uint8_t*)"0" : (uint8_t*)"1");
    }

    ISL_StatusTypeDef FSC_BT986::releaseConnections(uint16_t timeout) {
        return writeAT("DSCA", timeout);
    }

    ISL_StatusTypeDef FSC_BT986::restore(uint16_t timeout) {
        return writeAT("RESTORE", timeout);
    }



    ISL_StatusTypeDef FSC_BT986::hardReset() {
        if (pins.reset.isValid()) {
            pins.reset.write(0);
            system::Delay(DEFAULT_PIN_TIMEOUT);
            pins.reset.write(1);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::setHardMode(uint8_t state) {
        if (pins.mode.isValid()) {
            pins.mode.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    ISL_StatusTypeDef FSC_BT986::readStatus(uint8_t* rxbuff) {
        if (pins.status.isValid()) {
            *rxbuff = pins.status.read();
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::hardDisconnect() {
        if (pins.disconnect.isValid()) {
            pins.disconnect.write(1);
            system::Delay(DEFAULT_PIN_TIMEOUT);
            pins.disconnect.write(0);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

}

#endif
