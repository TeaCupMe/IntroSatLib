#define ISL_INTERNAL

#include "Adapter/UART.h"

#ifdef ISL_UART_ENABLED

#include "E32_433.h"


namespace IntroSatLib {

    ISL_StatusTypeDef E32_433::Init(Mode mode, uint16_t timeout)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));

        RETURN_STATUS_IF_NOT_OK_SILENT(SetMode(Mode::Sleep));

        uint8_t buff[6];
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadSettings(buff, timeout));
        currentSettings = buff;

        RETURN_STATUS_IF_NOT_OK_SILENT(SetMode(mode));
        
        return ISL_OK;  
    }

    ISL_StatusTypeDef E32_433::SetMode(Mode mode)
    {
        if (!IsReady()) return ISL_BUSY;

        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1));
        if (pins.M0.isValid() && pins.M1.isValid())
        {
            pins.M0.write((uint8_t)mode & 0b1);
            pins.M1.write((uint8_t)mode & 0b10);
            RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(0));
            RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1));
            currentMode = mode;
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::WaitAUX( uint8_t level, uint16_t timeout)
    {
        uint32_t start;
        if (pins.AUX.isValid()) {
            start = system::GetTick();
            while(pins.AUX.read() != level) {
                if((system::GetTick() - start) > timeout) return ISL_TIMEOUT;
            }
            system::Delay(2);
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::TransmitLoRa(uint8_t* txbuff, uint16_t length, uint16_t timeout)
    {
        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        system::Delay(10);

        const uint16_t BLOCK_SIZE = 512;

        uint8_t *ptr = txbuff;
        for (uint8_t i = 0; i < (length / BLOCK_SIZE); i++ )
        {
            RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(ptr, BLOCK_SIZE, timeout));
            RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
            ptr += BLOCK_SIZE;
        }

        uint16_t tail = length % BLOCK_SIZE;
        if (tail)
        {
            RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(ptr, tail, timeout));
            RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        }

        lastTransactionTime = system::GetTick();
        transactionCompleteTimeout = txDelay * ((length / 58) + 1);
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::ReceiveLoRa(uint8_t* rxbuff, uint16_t length, uint16_t timeout)
    {
        if (!IsReady()) return ISL_BUSY;

        uint16_t itr = 0;
        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(0, timeout));

        while((Available() || pins.AUX.waitReset(timeout) == ISL_OK) && itr < length) {
            ReadUART(rxbuff + itr, 1, timeout);
            itr++;
        }

        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::ReadSettingsRaw(uint8_t* rxbuff, uint16_t timeout)
    {
        if (currentMode != Mode::Sleep) return ISL_ERROR;

        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        FlushRX();

        uint8_t message[3] = {
            (uint8_t)CommandHead::GetParameters,
            (uint8_t)CommandHead::GetParameters,
            (uint8_t)CommandHead::GetParameters
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 6, timeout));
        if (rxbuff[0] == (uint8_t)CommandHead::SetParametersSave)
        {
            currentSettings = rxbuff;
            return ISL_OK;
        }
        return ISL_ERROR;
    }

    ISL_StatusTypeDef E32_433::ReadSettings(uint8_t* rxbuff, uint16_t timeout)
    {
        return ReadSettingsRaw(rxbuff, timeout);
    }

    ISL_StatusTypeDef E32_433::ReadSettings(E32Settings& settings, uint16_t timeout)
    {
        uint8_t buffer[6];
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadSettingsRaw(buffer, timeout));
        settings = buffer;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::ReadVersion(uint8_t* rxbuff, uint16_t timeout)
    {
        if (currentMode != Mode::Sleep) return ISL_ERROR;

        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        FlushRX();

        uint8_t message[3] = {
            (uint8_t)CommandHead::GetVersion, 
            (uint8_t)CommandHead::GetVersion, 
            (uint8_t)CommandHead::GetVersion
        };
        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(rxbuff, 4, timeout));
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::Reset(uint16_t timeout)
    {
        if (currentMode != Mode::Sleep) return ISL_ERROR;

        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
    	uint8_t message[3] = {
            (uint8_t)CommandHead::Reset, 
            (uint8_t)CommandHead::Reset, 
            (uint8_t)CommandHead::Reset
        };
		RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(message, 3, timeout));
        return ISL_OK; 
    }

    ISL_StatusTypeDef E32_433::SetSettings(E32Settings settings, uint16_t timeout)
    {
        if (currentMode != Mode::Sleep) return ISL_ERROR;

        RETURN_STATUS_IF_NOT_OK_SILENT(WaitAUX(1, timeout));
        FlushRX();

        uint8_t data[6];

        data[0] = settings.head;
        data[1] = (uint8_t)(settings.addr >> 8);
        data[2] = (uint8_t)settings.addr;
        data[3] = settings.sped;
        data[4] = settings.chan;
        data[5] = settings.option;

        RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(data, 6, timeout));
        
        RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(data, 6, timeout));
        if (data[0] == (uint8_t)CommandHead::SetParametersSave)
        {
            currentSettings = data;
            return ISL_OK;
        }
        return ISL_ERROR;
    }


    ISL_StatusTypeDef E32_433::SetAddr(uint16_t addr, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.addr = addr;
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::SetSPEDByte(uint8_t sped, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = sped;
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetUARTParity(UARTParity parity, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~(0b11 << 6)) | static_cast<uint8_t>(parity);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetUARTBaudrate(UARTBaudrate baudrate, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~(0b111 << 3)) | static_cast<uint8_t>(baudrate);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetAirDatarate(AirDatarate datarate, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.sped = (data.sped & ~0b111) | static_cast<uint8_t>(datarate);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::SetChannel(uint8_t channel, uint16_t timeout)
    {
        if (channel > maxChannel) return ISL_ERROR;

        E32Settings data = currentSettings;
        data.chan = channel;
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }


    ISL_StatusTypeDef E32_433::SetOPTIONByte(uint8_t option, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = option;
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetFixedTransmission(AddressingMode mode, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 7)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetIODriveMode(IODriveMode mode, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 6)) | static_cast<uint8_t>(mode);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetWakeUpTime(WakeUpTime wtime, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b111 << 3)) | static_cast<uint8_t>(wtime);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetFEC(FEC fec, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~(0b1 << 2)) | static_cast<uint8_t>(fec);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

    ISL_StatusTypeDef E32_433::SetTxPower(TxPower power, uint16_t timeout)
    {
        E32Settings data = currentSettings;
        data.option = (data.option & ~0b11) | static_cast<uint8_t>(power);
        RETURN_STATUS_IF_NOT_OK_SILENT(SetSettings(data, timeout));
        currentSettings = data;
        return ISL_OK;
    }

}

#endif
