#define ISL_INTERNAL

#include "Adapter/UART.h"

#ifdef ISL_UART_ENABLED

#include "E32-433.h"


namespace IntroSatLib {

    ISL_StatusTypeDef E32_433::Init()
    {
        waitAUX();

        RETURN_STATUS_IF_NOT_OK_SILENT(setMode(currentMode));
    
        // E32_SettingsBytes data;
        uint8_t buff[6];
        RETURN_STATUS_IF_NOT_OK_SILENT(readSettings(buff, DEFAULT_TIMEOUT));
        currentSettings = buff;
        
        return ISL_OK;  
    }

    ISL_StatusTypeDef E32_433::setMode(MODE mode)
    {
        waitAUX();
        if (pins.M0.isValid() && pins.M1.isValid())
        {
            pins.M0.write((uint8_t)mode & 0b1);
            pins.M1.write((uint8_t)mode & 0b10);
            currentMode = mode;
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::waitAUX(uint16_t timeout)
    {
        uint32_t start;
        if (pins.AUX.isValid()) {
            start = system::GetTick();
            while(!pins.AUX.read()) {
                if((system::GetTick() - start) > timeout) return ISL_TIMEOUT;
            }
        }
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::readSettingsRaw(uint8_t* rxbuff, uint16_t timeout)
    {
        uint8_t buff;
        waitAUX(timeout);
        while(available()) ReadUART(&buff, 1, 10);

        uint8_t message[3] = {
            (uint8_t)CMD_HEAD::GET_PARAMETERS,
            (uint8_t)CMD_HEAD::GET_PARAMETERS,
            (uint8_t)CMD_HEAD::GET_PARAMETERS
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        waitAUX(timeout);
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 6, timeout));
        if (rxbuff[0] == (uint8_t)CMD_HEAD::SET_PARAMETERS_SAVE)
        {
            currentSettings = rxbuff;
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef E32_433::readSettings(uint8_t* rxbuff, uint16_t timeout)
    {
        return readSettingsRaw(rxbuff, timeout);
    }

    ISL_StatusTypeDef E32_433::readSettings(E32_Settings& settings, uint16_t timeout)
    {
        uint8_t buffer[6];
        RETURN_STATUS_IF_NOT_OK_SILENT(readSettingsRaw(buffer, timeout));
        settings = buffer;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::readVersion(uint8_t* rxbuff, uint16_t timeout)
    {
        uint8_t buff;
        waitAUX(timeout);
        while(available()) ReadUART(&buff, 1, 10);

        uint8_t message[3] = {
            (uint8_t)CMD_HEAD::GET_VERSION, 
            (uint8_t)CMD_HEAD::GET_VERSION, 
            (uint8_t)CMD_HEAD::GET_VERSION
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 4, timeout));
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::reset(uint16_t timeout)
    {
        waitAUX(timeout);
    	uint8_t message[3] = {
            (uint8_t)CMD_HEAD::SET_RESET, 
            (uint8_t)CMD_HEAD::SET_RESET, 
            (uint8_t)CMD_HEAD::SET_RESET
        };
		RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        return ISL_OK; 
    }

    ISL_StatusTypeDef E32_433::setSettings(E32_Settings settings, uint16_t timeout)
    {
        uint8_t buff;
        waitAUX(timeout);
        while(available()) ReadUART(&buff, 1, 10);

        uint8_t data[6];
        settings.toBytes(data);
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data, 6, timeout));
        waitAUX(timeout);
        
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(data, 6, timeout));
        if (data[0] == (uint8_t)CMD_HEAD::SET_PARAMETERS_SAVE)
        {
            currentSettings = data;
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::setAddr(uint16_t addr, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.ADDR = addr;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setSPEDByte(uint8_t sped, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.SPED = sped;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUARTParity(UART_PARITY parity, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.SPED = (data.SPED & ~(0b11 << 6)) | static_cast<uint8_t>(parity);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUARTBaudrate(UART_BAUDRATE baudrate, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.SPED = (data.SPED & ~(0b111 << 3)) | static_cast<uint8_t>(baudrate);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setAirDatarate(AIR_DATARATE datarate, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.SPED = (data.SPED & ~0b111) | static_cast<uint8_t>(datarate);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setChannel(uint8_t channel, uint16_t timeout)
    {
        if (channel > MAX_CHANNEL) return ISL_ERROR;

        E32_Settings data = currentSettings;
        data.CHAN = channel;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setOPTIONByte(uint8_t option, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = option;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFixedTransmission(TRANSMISSION_MODE mode, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = (data.OPTION & ~(0b1 << 7)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setIODriveMode(IO_DRIVE_MODE mode, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = (data.OPTION & ~(0b1 << 6)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setWakeUpTime(WAKE_UP_TIME wtime, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = (data.OPTION & ~(0b111 << 3)) | static_cast<uint8_t>(wtime);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFEC(FEC fec, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = (data.OPTION & ~(0b1 << 2)) | static_cast<uint8_t>(fec);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setTXPower(TX_POWER power, uint16_t timeout)
    {
        E32_Settings data = currentSettings;
        data.OPTION = (data.OPTION & ~0b11) | static_cast<uint8_t>(power);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

}

#endif
