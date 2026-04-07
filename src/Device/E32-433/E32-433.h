#ifndef E32_433_H_
#define E32_433_H_

#include "Device/UARTDevice.h"
#include "Adapter/GPIO.h"


namespace IntroSatLib
{
    
class E32_433 : public UARTDevice
{
public:

    struct E32Pins
    {
        interfaces::GPIO M0;
        interfaces::GPIO M1;
        interfaces::GPIO AUX;
    };

    enum class Mode: uint8_t
    {
        Normal = 0,
        Wakeup = 1,
        PowerSave = 2,
        Sleep = 3
    };



    typedef struct __attribute__((packed)) E32Settings
    {
        uint8_t head = 0xC0;
        uint16_t addr = 0x00;
        uint8_t sped = 0x1A;
        uint8_t chan = 0x17;
        uint8_t option = 0x40;

        E32Settings() { }

        E32Settings(uint8_t* buff)
        : head(buff[0]), 
          addr(((uint16_t)buff[1]) << 8 | buff[2]), 
          sped(buff[3]), 
          chan(buff[4]), 
          option(buff[5])
        { }

        E32Settings(uint8_t head, uint16_t addr, uint8_t sped, uint8_t chan, uint8_t option)
        : head(head), addr(addr), sped(sped), chan(chan), option(option)
        { }

        E32Settings& operator=(uint8_t* bytes)
        {
            head = bytes[0];
            addr = ((uint16_t)bytes[1]) << 8 | bytes[2];
            sped = bytes[3];
            chan = bytes[4];
            option = bytes[5];
            return *this;
        }

    };

    // ================================================
    //                      HEAD
    // ================================================
    enum class CommandHead : uint8_t
    {
		SetParametersSave = 0xC0,
		GetParameters,
		SetParametersNoSave,
		GetVersion,
		Reset
	};

    // ================================================
    //                      SPED
    // ================================================
    enum class UARTParity : uint8_t
    {
		None = 0 << 6,
        Odd = 0b01 << 6, 
        Even = 0b10 << 6
	};
	enum class UARTBaudrate : uint8_t
    {
		BR_1200 = 0,
		BR_2400 = 0b001 << 3,
		BR_4800 = 0b010 << 3,
		BR_9600 = 0b011 << 3,
		BR_19200 = 0b100 << 3,
		BR_38400 = 0b101 << 3,
		BR_57600 = 0b110 << 3,
		BR_115200 = 0b111 << 3
	};
	enum class AirDatarate : uint8_t
    {
		DR_300 = 0,
		DR_1200 = 0b001,
		DR_2400 = 0b010,
		DR_4800 = 0b011,
		DR_9600 = 0b100,
		DR_19200 = 0b101
	};

    // ================================================
    //                    OPTION
    // ================================================
	enum class AddressingMode : uint8_t
    {
		Transparent = 0,
        Fixed = 0b1 << 7
	};
	enum class IODriveMode : uint8_t
    {
		Opencollector = 0,
        Pushpull = 0b1 << 6
	};
	enum class WakeUpTime : uint8_t
    {
		T_250ms = 0,
		T_500ms = 0b001 << 3,
		T_750ms = 0b010 << 3,
		T_1000ms = 0b011 << 3,
		T_1250ms = 0b100 << 3,
		T_1500ms = 0b101 << 3,
		T_1750ms = 0b110 << 3,
		T_2000ms = 0b111 << 3
	};
	enum class FEC : uint8_t
    {
		Disabled = 0,
        Enabled = 0b1 << 2
	};
	enum class TxPower : uint8_t
    {
		Max = 0,
        Mid = 0b01,
        Low = 0b10,
        Eco = 0b11
	};
    

private:

    static constexpr uint8_t maxChannel = 0x1F;
    static constexpr uint16_t defaultBSize = 1024;
    static constexpr uint16_t txDelay = 350;

    uint32_t lastTransactionTime = 0;
    uint32_t transactionCompleteTimeout = 0;

    E32Pins pins;
    E32Settings currentSettings;
    Mode currentMode = Mode::Normal;
    uint8_t LoRaRxBuff[defaultBSize];

    ISL_StatusTypeDef WaitAUX(uint8_t level, uint16_t timeout=defaultTimeout);
    void WaitForReady(uint32_t afterAuxTime);

protected:

    static constexpr uint16_t defaultTimeout = 1000;

    ISL_StatusTypeDef ReadSettingsRaw(uint8_t* rxbuff, uint16_t timeout);

public:

    E32_433(
        interfaces::UART uart, 
        E32Pins pins
    ): UARTDevice(uart), pins(pins) {}

    ISL_StatusTypeDef Init() { return Init(Mode::Normal); }
    ISL_StatusTypeDef Init(Mode mode, uint16_t timeout = defaultTimeout);

    ISL_StatusTypeDef TransmitLoRa(uint8_t* txbuff, uint16_t length, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef ReceiveLoRa(uint8_t* rxbuff, uint16_t length, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef SetMode(Mode mode);

    ISL_StatusTypeDef ReadSettings(uint8_t* rxbuff, uint16_t timeout = defaultTimeout);
    ISL_StatusTypeDef ReadSettings(E32Settings& settings, uint16_t timeout = defaultTimeout);

    ISL_StatusTypeDef ReadVersion(uint8_t* rxbuff, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef Reset(uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef SetSettings(E32Settings settings, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef SetAddr(uint16_t addr, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef SetSPEDByte(uint8_t sped, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetUARTParity(UARTParity parity = UARTParity::None, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetUARTBaudrate(UARTBaudrate baudrate = UARTBaudrate::BR_9600, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetAirDatarate(AirDatarate datarate = AirDatarate::DR_2400, uint16_t timeout=defaultTimeout);

	ISL_StatusTypeDef SetChannel(uint8_t channel, uint16_t timeout=defaultTimeout);

    ISL_StatusTypeDef SetOPTIONByte(uint8_t option, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetFixedTransmission(AddressingMode mode = AddressingMode::Transparent, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef SetIODriveMode(IODriveMode mode = IODriveMode::Opencollector, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetWakeUpTime(WakeUpTime wtime = WakeUpTime::T_250ms, uint16_t timeout=defaultTimeout);
	ISL_StatusTypeDef SetFEC(FEC fec = FEC::Enabled, uint16_t timeout=defaultTimeout);
    ISL_StatusTypeDef SetTxPower(TxPower power = TxPower::Max, uint16_t timeout=defaultTimeout);

};

} // namespace IntroSatLib

#endif // E32_433_H_
