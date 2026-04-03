#define ISL_INTERNAL

#include "Adapter/UART.h"

#ifdef ISL_UART_ENABLED

#include "E32-433.h"


namespace IntroSatLib {

    ISL_StatusTypeDef E32_433::Init()
    {
        waitAUX();

        RETURN_STATUS_IF_NOT_OK_SILENT(setMode(currentMode));

        uint8_t buff[6];
        RETURN_STATUS_IF_NOT_OK_SILENT(readSettings(buff, defaultTimeout));
        currentSettings = buff;
        
        return ISL_OK;  
    }

    ISL_StatusTypeDef E32_433::setMode(Mode mode)
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
            system::Delay(2);
        }
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::readSettingsRaw(uint8_t* rxbuff, uint16_t timeout)
    {
        uint8_t buff;
        waitAUX(timeout);
        while(available()) ReadUART(&buff, 1, 10);

        uint8_t message[3] = {
            (uint8_t)CommandHead::GetParameters,
            (uint8_t)CommandHead::GetParameters,
            (uint8_t)CommandHead::GetParameters
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        waitAUX(timeout);
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 6, timeout));
        if (rxbuff[0] == (uint8_t)CommandHead::SetParametersSave)
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

    ISL_StatusTypeDef E32_433::readSettings(E32Settings& settings, uint16_t timeout)
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
            (uint8_t)CommandHead::GetVersion, 
            (uint8_t)CommandHead::GetVersion, 
            (uint8_t)CommandHead::GetVersion
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 4, timeout));
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::reset(uint16_t timeout)
    {
        waitAUX(timeout);
    	uint8_t message[3] = {
            (uint8_t)CommandHead::Reset, 
            (uint8_t)CommandHead::Reset, 
            (uint8_t)CommandHead::Reset
        };
		RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        return ISL_OK; 
    }

    ISL_StatusTypeDef E32_433::setSettings(E32Settings settings, uint16_t timeout)
    {
        uint8_t buff;
        waitAUX(timeout);
        while(available()) ReadUART(&buff, 1, 10);

        uint8_t data[6];

        data[0] = settings.head;
        data[1] = (uint8_t)(settings.addr >> 8);
        data[2] = (uint8_t)settings.addr;
        data[3] = settings.sped;
        data[4] = settings.chan;
        data[5] = settings.option;

        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data, 6, timeout));
        waitAUX(timeout);
        
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(data, 6, timeout));
        if (data[0] == (uint8_t)CommandHead::SetParametersSave)
        {
            currentSettings = data;
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::setAddr(uint16_t addr, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.addr = addr;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setSPEDByte(uint8_t sped, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = sped;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUARTParity(UARTParity parity, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~(0b11 << 6)) | static_cast<uint8_t>(parity);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setUARTBaudrate(UARTBaudrate baudrate, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~(0b111 << 3)) | static_cast<uint8_t>(baudrate);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setAirDatarate(AirDatarate datarate, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~0b111) | static_cast<uint8_t>(datarate);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setChannel(uint8_t channel, uint16_t timeout)
    {
        if (channel > maxChannel) return ISL_ERROR;

        E32Settings data = currentSettings;
        data.chan = channel;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::setOPTIONByte(uint8_t option, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = option;
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFixedTransmission(AddressingMode mode, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 7)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setIODriveMode(IODriveMode mode, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 6)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setWakeUpTime(WakeUpTime wtime, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b111 << 3)) | static_cast<uint8_t>(wtime);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setFEC(FEC fec, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 2)) | static_cast<uint8_t>(fec);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::setTxPower(TxPower power, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~0b11) | static_cast<uint8_t>(power);
        RETURN_STATUS_IF_NOT_OK_SILENT(setSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

}

#endif