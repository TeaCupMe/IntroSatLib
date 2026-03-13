#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC-BT986.h"

namespace IntroSatLib {

    ISL_StatusTypeDef FSC_BT986::Init() {

        //TODO 
        // RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "PIOCFG", 
        //             (pins.mode.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0",
        //             (pins.disconnect.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0"
        //         ));


        writeHardMode(1);
        writePrgMode(1);

        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "BTEN", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "LPM", (uint8_t*)"0"));
        // RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "COD", (uint8_t*)"240404"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "FLOWCTL", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "SSP", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "MODE", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "SECURITY", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT((uint8_t*) "SPPCFG", (uint8_t*)"0"));

        writeHardMode(0);
        writePrgMode(0);
        
        return ISL_OK;
    }
    
    ISL_StatusTypeDef FSC_BT986::writePIN(uint8_t* pinCode, uint16_t timeout) {
        return writeAT((uint8_t*)"PIN", pinCode, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::readPIN(uint8_t* rxbuff, uint16_t timeout) {
        return readAT((uint8_t*)"PIN", rxbuff, 30, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::readMAC(uint8_t* rxbuff, uint16_t timeout) {
        return readAT((uint8_t*)"ADDR", rxbuff, 28, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::connect(uint8_t* mac, uint16_t timeout) {
        return writeAT((uint8_t*)"SPPCONN", mac, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::writePrgReboot(uint16_t timeout) {
        return writeAT((uint8_t*)"REBOOT", nullptr, timeout);
    }

    ISL_StatusTypeDef FSC_BT986::writePrgMode(uint8_t state, uint16_t timeout) {
        return writeAT((uint8_t*)"TPMODE", (state == 0) ? (uint8_t*)"0" : (uint8_t*)"1", timeout);
    }




    ISL_StatusTypeDef FSC_BT986::writeHardReboot(uint8_t state) {
        if (pins.reset.isValid()) {
            pins.reset.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    ISL_StatusTypeDef FSC_BT986::writeHardMode(uint8_t state) {
        if (pins.mode.isValid()) {
            pins.mode.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    uint8_t FSC_BT986::readStatus() {
        pins.status.read();
    }

    ISL_StatusTypeDef FSC_BT986::writeHardDisconnect(uint8_t state) {
        if (pins.disconnect.isValid()) {
            pins.disconnect.write(state);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

}

#endif
