#define ISL_INTERNAL

#include "Adapter/UART.h"

#ifdef ISL_UART_ENABLED

#include "E32-433.h"


namespace IntroSatLib {

    ISL_StatusTypeDef E32_433::Init() {
        RETURN_STATUS_IF_NOT_OK_SILENT(setMode(current_mode));
    
        E32_SettingsBytes data;
        RETURN_STATUS_IF_NOT_OK_SILENT(readParams(data.bytes, DEFAULT_TIMEOUT));
        current_settings = data.settings;
        
        return ISL_OK;  
    }

    ISL_StatusTypeDef E32_433::setMode(MODE mode)
    {
        if (pins.M0.isValid() && pins.M1.isValid()) {
            pins.M0.write(mode & 0b1);
            pins.M1.write(mode & 0b10);
            current_mode = mode;
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef E32_433::readParams(uint8_t* rxbuff, uint16_t timeout)
    {
        uint8_t buff[6];
        uint8_t message[3] = {GET_PARAMETERS, GET_PARAMETERS, GET_PARAMETERS};
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 6, timeout));
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::readVersion(uint8_t* rxbuff, uint16_t timeout)
    {
        uint8_t buff[4];
        uint8_t message[3] = {GET_VERSION, GET_VERSION, GET_VERSION};
                RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 4, timeout));
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::reset(uint16_t timeout)
    {
    	uint8_t message[3] = {SET_RESET, SET_RESET, SET_RESET};
		RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        return ISL_OK; 
    }

    ISL_StatusTypeDef E32_433::setSettings(E32_Settings *settings, uint16_t timeout) {
        E32_SettingsBytes data = { *settings };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setAddr(uint16_t addr, uint16_t timeout) {
        E32_SettingsBytes data = { current_settings };
        data.settings.ADDR = addr;
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setSPEDByte(uint8_t sped, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.SPED = sped;
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUartParity(UART_PARITY parity, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.SPED = (data.settings.SPED & ~(0b11 << 6)) | static_cast<uint8_t>(parity);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUARTBaudrate(UART_BAUDRATE baudrate, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.SPED = (data.settings.SPED & ~(0b111 << 3)) | static_cast<uint8_t>(baudrate);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setAirDatarate(AIR_DATARATE datarate, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.SPED = (data.settings.SPED & ~0b111) | static_cast<uint8_t>(datarate);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setChannel(uint8_t channel, uint16_t timeout)
    {
        if (channel > 0x1F) { return ISL_ERROR; }
        
        E32_SettingsBytes data = { current_settings };
        data.settings.CHAN = channel;
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setOPTIONByte(uint8_t option, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = option;
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFixedTransmission(TRANSMISSION_MODE mode, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = (data.settings.OPTION & ~(1 << 7)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setIODriveMode(IO_DRIVE_MODE mode, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = (data.settings.OPTION & ~(1 << 6)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setWakeUpTime(WAKE_UP_TIME wtime, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = (data.settings.OPTION & ~(0b111 << 3)) | static_cast<uint8_t>(wtime);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFEC(FEC fec, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = (data.settings.OPTION & ~(1 << 2)) | static_cast<uint8_t>(fec);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setTXPower(TX_POWER power, uint16_t timeout)
    {
        E32_SettingsBytes data = { current_settings };
        data.settings.OPTION = (data.settings.OPTION & ~0b11) | static_cast<uint8_t>(power);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data.bytes, 6, timeout));
        current_settings = data.settings;
        return ISL_OK;
    }

}

#endif
