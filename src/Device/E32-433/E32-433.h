#ifndef E32_433_H_
#define E32_433_H_

#include "Device/UARTDevice.h"
#include "Adapter/GPIO.h"


namespace IntroSatLib
{
    
class E32_433 : public UARTDevice
{
public:

    struct E32_433_pins
    {
        interfaces::GPIO M0;
        interfaces::GPIO M1;
        interfaces::GPIO AUX;
    };

    enum class MODE: uint8_t
    {
        NORMAL = 0,
        WAKEUP = 1,
        PWRSAVE = 2,
        SLEEP = 3
    };



    typedef struct __attribute__((packed)) E32_Settings
    {
        uint8_t HEAD = 0xC0;
        uint16_t ADDR = 0x00;
        uint8_t SPED = 0x1A;
        uint8_t CHAN = 0x17;
        uint8_t OPTION = 0x40;

        E32_Settings() {}

        E32_Settings(uint8_t* buff)
        : HEAD(buff[0]), 
          ADDR(((uint16_t)buff[1]) << 8 | buff[2]), 
          SPED(buff[3]), 
          CHAN(buff[4]), 
          OPTION(buff[5])
        { }

        E32_Settings(uint8_t head, uint16_t addr, uint8_t sped, uint8_t chan, uint8_t option)
        : HEAD(head), ADDR(addr), SPED(sped), CHAN(chan), OPTION(option)
        { }

        E32_Settings& operator=(uint8_t* bytes)
        {
            HEAD = bytes[0];
            ADDR = ((uint16_t)bytes[1]) << 8 | bytes[2];
            SPED = bytes[3];
            CHAN = bytes[4];
            OPTION = bytes[5];
            return *this;
        }

        void toBytes(uint8_t* buff)
        {
            buff[0] = HEAD;
            buff[1] = (uint8_t)(ADDR >> 8);
            buff[2] = (uint8_t)ADDR;
            buff[3] = SPED;
            buff[4] = CHAN;
            buff[5] = OPTION;
            return;
        }

    };

    // ================================================
    //                      HEAD
    // ================================================
    enum class CMD_HEAD : uint8_t
    {
		SET_PARAMETERS_SAVE = 0xC0,
		GET_PARAMETERS,
		SET_PARAMETERS_NO_SAVE,
		GET_VERSION,
		SET_RESET
	};

    // ================================================
    //                      SPEAD
    // ================================================
    enum class UART_PARITY : uint8_t
    {
		PARITY_8N1 = 0 << 6,
        PARITY_8O1 = 0b01 << 6, 
        PARITY_8E1 = 0b10 << 6
	};
	enum class UART_BAUDRATE : uint8_t
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
	enum class AIR_DATARATE : uint8_t
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
	enum class TRANSMISSION_MODE : uint8_t
    {
		M_TRANSPARENT = 0,
        M_FIXED = 0b1 << 7
	};
	enum class IO_DRIVE_MODE : uint8_t
    {
		IO_OPENCOLLECTOR = 0,
        IO_PUSHPULL = 0b1 << 6
	};
	enum class WAKE_UP_TIME : uint8_t
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
		FEC_DISABLED = 0,
        FEC_ENABLED = 0b1 << 2
	};
	enum class TX_POWER : uint8_t
    {
		P_MAX = 0,
        P_MID = 0b01,
        P_LOW = 0b10,
        P_ECO = 0b11
	};
    

private:

    E32_433_pins pins;
    E32_Settings currentSettings;
    MODE currentMode;


    static constexpr uint8_t MAX_CHANNEL = 0x1F;

protected:

public:

    E32_433(
        interfaces::UART uart, 
        E32_433_pins _pins,
        MODE mode = MODE::NORMAL
    ): UARTDevice(uart), pins(_pins), currentMode(mode) {}

    ISL_StatusTypeDef Init();

    ISL_StatusTypeDef setMode(MODE mode);

    ISL_StatusTypeDef readParams(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef readVersion(uint8_t* rxbuff, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef reset(uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef setSettings(E32_Settings settings, uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef setAddr(uint16_t addr, uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef setSPEDByte(uint8_t sped, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setUARTParity(UART_PARITY parity = UART_PARITY::PARITY_8N1, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setUARTBaudrate(UART_BAUDRATE baudrate = UART_BAUDRATE::BR_9600, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setAirDatarate(AIR_DATARATE datarate = AIR_DATARATE::DR_2400, uint16_t timeout=DEFAULT_TIMEOUT);

	ISL_StatusTypeDef setChannel(uint8_t channel, uint16_t timeout=DEFAULT_TIMEOUT);

    ISL_StatusTypeDef setOPTIONByte(uint8_t option, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setFixedTransmission(TRANSMISSION_MODE mode = TRANSMISSION_MODE::M_TRANSPARENT, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef setIODriveMode(IO_DRIVE_MODE mode = IO_DRIVE_MODE::IO_OPENCOLLECTOR, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setWakeUpTime(WAKE_UP_TIME wtime = WAKE_UP_TIME::T_250ms, uint16_t timeout=DEFAULT_TIMEOUT);
	ISL_StatusTypeDef setFEC(FEC fec = FEC::FEC_ENABLED, uint16_t timeout=DEFAULT_TIMEOUT);
    ISL_StatusTypeDef setTXPower(TX_POWER power = TX_POWER::P_MAX, uint16_t timeout=DEFAULT_TIMEOUT);

};

} // namespace IntroSatLib

#endif E32_433_H_
