#define ISL_INTERNAL

#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#if defined(ISL_UART_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "FSC_BT986.h"

namespace IntroSatLib {

    ISL_StatusTypeDef FSC_BT986::SetMode(Mode mode, bool withSave)
    {
        switch (modeChange) {
            case ModeChange::None:
                return ISL_OK;
                break;
            case ModeChange::Hardware:
                RETURN_STATUS_IF_NOT_OK_SILENT(SetModeHard(mode));
                break;
            case ModeChange::Software:
                RETURN_STATUS_IF_NOT_OK_SILENT(SetModePrg(mode));
                break;
            case ModeChange::HardwareAndSoftware:
                RETURN_STATUS_IF_NOT_OK_SILENT(SetModeHard(mode));
                RETURN_STATUS_IF_NOT_OK_SILENT(SetModePrg(mode));
                break;
        }
        if (withSave) currentMode = mode;
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::SetModeHard(Mode mode)
    {
        if (pins.mode.isValid()) {
            pins.mode.write((uint8_t)mode);
            return ISL_OK;
        } else {return ISL_ERROR;}
    }

    ISL_StatusTypeDef FSC_BT986::SetModePrg(Mode mode, uint16_t timeout)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("TPMODE", timeout, mode==Mode::Throughput ? (uint8_t*)"0" : (uint8_t*)"1"));
        return ISL_OK;
    }



    ISL_StatusTypeDef FSC_BT986::Init(Mode mode)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("PIOCFG",
                    (pins.mode.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0",
                    (pins.disconnect.isValid()) ? (uint8_t*)"1" : (uint8_t*)"0"
                ));

        modeChange = (pins.mode.isValid()) ? 
                                ModeChange::HardwareAndSoftware : ModeChange::Software;

        SetMode(Mode::Command, false);

        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("BTEN", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("LPM", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("FLOWCTL", (uint8_t*)"0"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("SSP", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("MODE", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("SECURITY", (uint8_t*)"1"));
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("SPPCFG", (uint8_t*)"0"));

        SetMode(mode);
        
        return ISL_OK;
    }
    
    ISL_StatusTypeDef FSC_BT986::SetPIN(uint8_t* pinCode, uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("PIN", timeout, pinCode));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::ReadPIN(uint8_t* rxbuff, uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadAT("PIN", rxbuff, 30, timeout));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::ReadMAC(uint8_t* rxbuff, uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadAT("ADDR", rxbuff, 28, timeout));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::Connect(uint8_t* mac, uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("SPPCONN", timeout, mac));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::Reboot(uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("REBOOT", timeout));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::ReleaseConnections(uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("DSCA", timeout));
        SetMode(currentMode);
        return ISL_OK;
    }

    ISL_StatusTypeDef FSC_BT986::Restore(uint16_t timeout)
    {
        SetMode(Mode::Command, false);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteAT("RESTORE", timeout));
        SetMode(currentMode);
        return ISL_OK;
    }



    ISL_StatusTypeDef FSC_BT986::HardReset()
    {
        if (pins.reset.isValid()) {
            pins.reset.write(0);
            system::Delay(defaultPinTimeout);
            pins.reset.write(1);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::ReadStatus(uint8_t* rxbuff)
    {
        if (pins.status.isValid()) {
            *rxbuff = pins.status.read();
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef FSC_BT986::HardDisconnect()
    {
        if (pins.disconnect.isValid()) {
            pins.disconnect.write(1);
            system::Delay(defaultPinTimeout);
            pins.disconnect.write(0);
            return ISL_OK;
        }
        return ISL_ERROR;
    }

}

#endif
