#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC-BT986.h"

namespace IntroSatLib {

    ISL_StatusTypeDef FSC_BT986::setMode(MODE mode)
    {
        switch (modeChange) {
            case MODE_CHANGE::NONE:
                return ISL_OK;
                break;
            case MODE_CHANGE::HARDWARE:
                RETURN_STATUS_IF_NOT_OK_SILENT(setModeHard(mode));
                break;
            case MODE_CHANGE::SOFTWARE:
                RETURN_STATUS_IF_NOT_OK_SILENT(setModePrg(mode));
                break;
            case MODE_CHANGE::HARDWARE_AND_SOFTWARE:
                RETURN_STATUS_IF_NOT_OK_SILENT(setModeHard(mode));
                RETURN_STATUS_IF_NOT_OK_SILENT(setModePrg(mode));
                break;
        }
        currentMode = mode;
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::setModeHard(MODE mode)
    {
        if (pins.mode.isValid()) {
            pins.mode.write((uint8_t)mode);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    ISL_StatusTypeDef FSC_BT986::setModePrg(MODE mode, uint16_t timeout)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("TPMODE", timeout, mode==MODE::THROUGHPUT ? (uint8_t*)"0" : (uint8_t*)"1"));
        return ISL_OK;
    }



    ISL_StatusTypeDef FSC_BT986::Init(MODE mode)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("PIOCFG",
                    (pins.mode.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0",
                    (pins.disconnect.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0"
                ));

        modeChange = (pins.mode.isValid()) ? 
                                MODE_CHANGE::HARDWARE_AND_SOFTWARE : MODE_CHANGE::SOFTWARE;

        setMode(MODE::COMMAND);

        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("BTEN", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("LPM", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("FLOWCTL", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SSP", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("MODE", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SECURITY", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SPPCFG", (uint8_t*)"0"));

        setMode(mode);
        
        return ISL_OK;
    }
    
    ISL_StatusTypeDef FSC_BT986::setPIN(uint8_t* pinCode, uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("PIN", timeout, pinCode));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::readPIN(uint8_t* rxbuff, uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(readAT("PIN", rxbuff, 30, timeout));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::readMAC(uint8_t* rxbuff, uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(readAT("ADDR", rxbuff, 28, timeout));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::connect(uint8_t* mac, uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("SPPCONN", timeout, mac));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::reboot(uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("REBOOT", timeout));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::releaseConnections(uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("DSCA", timeout));
        setMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::restore(uint16_t timeout)
    {
        setMode(MODE::COMMAND);
        RETURN_STATUS_IF_NOT_OK_SILENT(writeAT("RESTORE", timeout));
        setMode(currentMode);
        return ISL_OK;
    }



    ISL_StatusTypeDef FSC_BT986::hardReset()
    {
        if (pins.reset.isValid()) {
            pins.reset.write(0);
            system::Delay(DEFAULT_PIN_TIMEOUT);
            pins.reset.write(1);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::readStatus(uint8_t* rxbuff)
    {
        if (pins.status.isValid()) {
            *rxbuff = pins.status.read();
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::hardDisconnect()
    {
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
