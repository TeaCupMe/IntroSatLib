/**
Based on https://github.com/mfurga/cc1101 by Mateusz Furga

Copyright (c) 2023 Mateusz Furga
2026 - Modified by Aleksey <TeaCupMe> Gilenko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef ISL_CC1101_H_
#define ISL_CC1101_H_

#include "Adapter/GPIO.h"
#include "Adapter/SPI.h"
#include "Device/SPIDevice.h"
#include "math.h"


namespace IntroSatLib {
class CC1101: SPIDevice {

private:
	constexpr double log2(double val) {
		return log(val) / log(2);
	}
	
	const uint8_t CC1101_FIFO_SIZE         = 64;    /* 64 B */
	const uint8_t CC1101_CRYSTAL_FREQ      = 26;    /* 26 MHz */

	const uint8_t CC1101_PARTNUM           = 0x00;
	const uint8_t CC1101_VERSION           = 0x14;
	const uint8_t CC1101_VERSION_LEGACY    = 0x04;

	enum CommandType {
		CC1101_CMD_TYPE_WRITE 	= 0x00,
		CC1101_CMD_TYPE_READ 	= 0x80,
		CC1101_CMD_TYPE_BURST	= 0x40
	};

	enum Command {
		CC1101_CMD_RES          = 0x30,  /* Reset chip */
		CC1101_CMD_RX           = 0x34,  /* Enable RX */
		CC1101_CMD_TX           = 0x35,  /* Enable TX */
		CC1101_CMD_IDLE         = 0x36,  /* Enable IDLE */
		CC1101_CMD_FRX          = 0x3a,  /* Flush the RX FIFO buffer */
		CC1101_CMD_FTX          = 0x3b,  /* Flush the TX FIFO buffer */
		CC1101_CMD_NOP          = 0x3d   /* No operation */
	};

	enum ConfigRegisterMap {
		CC1101_REG_IOCFG2 = 0x00,        // GDO2 output pin configuration
		CC1101_REG_IOCFG1 = 0x01,        // GDO1 output pin configuration
		CC1101_REG_IOCFG0 = 0x02,        // GDO0 output pin configuration
		CC1101_REG_FIFOTHR = 0x03,        // RX FIFO and TX FIFO thresholds
		CC1101_REG_SYNC1 = 0x04,        // Sync word, high INT8U
		CC1101_REG_SYNC0 = 0x05,        // Sync word, low INT8U
		CC1101_REG_PKTLEN = 0x06,        // Packet length
		CC1101_REG_PKTCTRL1 = 0x07,        // Packet automation control
		CC1101_REG_PKTCTRL0 = 0x08,        // Packet automation control
		CC1101_REG_ADDR = 0x09,        // Device address
		CC1101_REG_CHANNR = 0x0A,        // Channel number
		CC1101_REG_FSCTRL1 = 0x0B,        // Frequency synthesizer control
		CC1101_REG_FSCTRL0 = 0x0C,        // Frequency synthesizer control
		CC1101_REG_FREQ2 = 0x0D,        // Frequency control word, high INT8U
		CC1101_REG_FREQ1 = 0x0E,        // Frequency control word, middle INT8U
		CC1101_REG_FREQ0 = 0x0F,        // Frequency control word, low INT8U
		CC1101_REG_MDMCFG4 = 0x10,        // Modem configuration
		CC1101_REG_MDMCFG3 = 0x11,        // Modem configuration
		CC1101_REG_MDMCFG2 = 0x12,        // Modem configuration
		CC1101_REG_MDMCFG1 = 0x13,        // Modem configuration
		CC1101_REG_MDMCFG0 = 0x14,        // Modem configuration
		CC1101_REG_DEVIATN = 0x15,        // Modem deviation setting
		CC1101_REG_MCSM2 = 0x16,   // Main Radio Control State Machine configuration
		CC1101_REG_MCSM1 = 0x17,   // Main Radio Control State Machine configuration
		CC1101_REG_MCSM0 = 0x18,   // Main Radio Control State Machine configuration
		CC1101_REG_FOCCFG = 0x19,     // Frequency Offset Compensation configuration
		CC1101_REG_BSCFG = 0x1A,        // Bit Synchronization configuration
		CC1101_REG_AGCCTRL2 = 0x1B,        // AGC control
		CC1101_REG_AGCCTRL1 = 0x1C,        // AGC control
		CC1101_REG_AGCCTRL0 = 0x1D,        // AGC control
		CC1101_REG_WOREVT1 = 0x1E,        // High INT8U Event 0 timeout
		CC1101_REG_WOREVT0 = 0x1F,        // Low INT8U Event 0 timeout
		CC1101_REG_WORCTRL = 0x20,        // Wake On Radio control
		CC1101_REG_FREND1 = 0x21,        // Front end RX configuration
		CC1101_REG_FREND0 = 0x22,        // Front end TX configuration
		CC1101_REG_FSCAL3 = 0x23,        // Frequency synthesizer calibration
		CC1101_REG_FSCAL2 = 0x24,        // Frequency synthesizer calibration
		CC1101_REG_FSCAL1 = 0x25,        // Frequency synthesizer calibration
		CC1101_REG_FSCAL0 = 0x26,        // Frequency synthesizer calibration
		CC1101_REG_RCCTRL1 = 0x27,        // RC oscillator configuration
		CC1101_REG_RCCTRL0 = 0x28,        // RC oscillator configuration
		CC1101_REG_FSTEST = 0x29,       // Frequency synthesizer calibration control
		CC1101_REG_PTEST = 0x2A,        // Production test
		CC1101_REG_AGCTEST = 0x2B,        // AGC test
		CC1101_REG_TEST2 = 0x2C,        // Various test settings
		CC1101_REG_TEST1 = 0x2D,        // Various test settings
		CC1101_REG_TEST0 = 0x2E,        // Various test settings
		CC1101_REG_PARTNUM =       0x30,
		CC1101_REG_VERSION =       0x31,
		CC1101_REG_TXBYTES =      0x3a,
		CC1101_REG_RXBYTES =      0x3b,
		CC1101_REG_PATABLE =      0x3e,
		CC1101_REG_RCCTRL0_STATUS = 0x3d,
		CC1101_REG_FIFO =           0x3f
	};

	enum StrobeCommand {
		CC1101_SRES = 0x30,       // Reset chip.
		CC1101_SFSTXON = 0x31, // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
							   // If in RX/TX: Go to a wait state where only the synthesizer is
							   // running (for quick RX / TX turnaround).
		CC1101_SXOFF = 0x32,       // Turn off crystal oscillator.
		CC1101_SCAL = 0x33,   // Calibrate frequency synthesizer and turn it off
							  // (enables quick start).
		CC1101_SRX = 0x34, // Enable RX. Perform calibration first if coming from IDLE and
						   // MCSM0.FS_AUTOCAL=1.
		CC1101_STX = 0x35, // In IDLE state: Enable TX. Perform calibration first if
						   // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
						   // Only go to TX if channel is clear.
		CC1101_SIDLE = 0x36, // Exit RX / TX, turn off frequency synthesizer and exit
							 // Wake-On-Radio mode if applicable.
		CC1101_SAFC = 0x37, // Perform AFC adjustment of the frequency synthesizer
		CC1101_SWOR = 0x38, // Start automatic RX polling sequence (Wake-on-Radio)
		CC1101_SPWD = 0x39,       // Enter power down mode when CSn goes high.
		CC1101_SFRX = 0x3A,       // Flush the RX FIFO buffer.
		CC1101_SFTX = 0x3B,       // Flush the TX FIFO buffer.
		CC1101_SWORRST = 0x3C,       // Reset real time clock.
		CC1101_SNOP = 0x3D, // No operation. May be used to pad strobe commands to two
							 // INT8Us for simpler software.
	};

public:
	enum Status {
		STATUS_OK = 0,
		STATUS_INVALID_PARAM,
		STATUS_CHIP_NOT_FOUND,
		STATUS_BAD_STATE,
		STATUS_LENGTH_TOO_SMALL,
		STATUS_LENGTH_TOO_BIG,
		STATUS_CRC_MISMATCH,
		STATUS_TXFIFO_UNDERFLOW,
		STATUS_RXFIFO_OVERFLOW
	};

	enum State {
		STATE_IDLE = 0, /* IDLE state */
		STATE_RX = 1, /* Receive mode */
		STATE_TX = 2, /* Transmit mode */
		STATE_FSTXON = 3, /* Fast TX ready */
		STATE_CALIBRATE = 4, /* Freq synthesizer calibration is running */
		STATE_SETTLING = 5, /* PLL is settling */
		STATE_RXFIFO_OVERFLOW = 6, /* RX FIFO has overflowed */
		STATE_TXFIFO_UNDERFLOW = 7, /* TX FIFO has underflowed */
	};

	enum Modulation {
		MOD_2FSK = 0, MOD_GFSK = 1, MOD_ASK_OOK = 3, MOD_4FSK = 4, MOD_MSK = 7
	};

	enum SyncMode {
		SYNC_MODE_NO_PREAMBLE = 0, /* No preamble/sync */
		SYNC_MODE_15_16 = 1, /* 15/16 sync word bits detected */
		SYNC_MODE_16_16 = 2, /* 16/16 sync word bits detected */
		SYNC_MODE_30_32 = 3, /* 30/32 sync word bits detected */
		SYNC_MODE_NO_PREAMBLE_CS = 4, /* No preamble/sync, CS above threshold */
		SYNC_MODE_15_16_CS = 5, /* 15/16 + carrier-sense above threshold */
		SYNC_MODE_16_16_CS = 6, /* 16/16 + carrier-sense above threshold */
		SYNC_MODE_30_32_CS = 7, /* 30/32 + carrier-sense above threshold */
	};

	enum PacketLengthMode {
		PKT_LEN_MODE_FIXED = 0, /* Length configured in PKTLEN register */
		PKT_LEN_MODE_VARIABLE = 1, /* Packet length put in the first byte */
	// TODO: PKT_LEN_MODE_INFINITE = 2,  /* Infinite packet length mode */
	};

	enum AddressFilteringMode {
		ADDR_FILTER_MODE_NONE = 0, /* No address check */
		ADDR_FILTER_MODE_CHECK = 1, /* Address check, no broadcast */
		ADDR_FILTER_MODE_CHECK_BC_0 = 2, /* Address check, 0 broadcast */
		ADDR_FILTER_MODE_CHECK_BC_0_255 = 3 /* Address check, 0 and 255 broadcast */
	};

	CC1101(interfaces::SPI _spi, interfaces::GPIO csPin): SPIDevice(_spi, csPin) {}

	Status Init(Modulation mod, double freq, double drate);

	ISL_StatusTypeDef Init() override {
		return Init(MOD_ASK_OOK, 433.5, 4) == Status::STATUS_OK ? ISL_StatusTypeDef::ISL_OK : ISL_StatusTypeDef::ISL_ERROR;
	}

	uint8_t getChipPartNumber();
	uint8_t getChipVersion();

	void setModulation(Modulation mod);
	Status setFrequency(double freq);
	Status setFrequencyDeviation(double dev);
	void setChannel(uint8_t ch);
	Status setChannelSpacing(double sp);
	Status setDataRate(double drate);
	Status setRxBandwidth(double bw);

	void setOutputPower(int8_t power);

	void setCrc(bool enable);
	void setDataWhitening(bool enable);
	Status setManchester(bool enable);
	Status setFEC(bool enable);
	void setAddressFilteringMode(AddressFilteringMode mode);
	void setPacketLengthMode(PacketLengthMode mode, uint8_t length = 255);
	void setSyncMode(SyncMode mode);
	Status setPreambleLength(uint8_t length);
	void setSyncWord(uint16_t sync);

	Status transmit(uint8_t *data, size_t length, uint8_t addr = 0);
	Status receive(uint8_t *data, size_t length, size_t *read = nullptr,
			uint8_t addr = 0);
	int8_t getRSSI();
	uint8_t getLQI();

	void receiveCallback(void (*func)(void));

private:
	void chipSelect();
	void chipDeselect();
	void waitReady();
	uint8_t waitForBytesInFifo();

	uint8_t readRegField(uint8_t addr, uint8_t hi, uint8_t lo);
	uint8_t readReg(uint8_t addr);
	void readRegBurst(uint8_t addr, uint8_t *buff, size_t size);

	void writeRegField(uint8_t addr, uint8_t data, uint8_t hi, uint8_t lo);
	void writeReg(uint8_t addr, uint8_t data);
	void writeRegBurst(uint8_t addr, uint8_t *data, size_t size);

	void sendCmd(uint8_t addr);

	void setRegs();
	void hardReset();
	void flushRxBuffer();
	void flushTxBuffer();

	State getState();
	void setState(State);
	void saveStatus(uint8_t status);

//	uint8_t gd0, gd2;
	uint8_t clk, miso, mosi;

	State currentState = STATE_IDLE;
	Modulation mod = MOD_2FSK;
	PacketLengthMode pktLenMode = PKT_LEN_MODE_FIXED;
	AddressFilteringMode addrFilterMode = ADDR_FILTER_MODE_NONE;
	bool recvCallback = false;

	double freq = 433.5;
	double drate = 4.0;
	int8_t power = 0;
	uint8_t pktLen;
	uint8_t rssi;
	uint8_t lqi;
	bool manchester = false, fec = false;

};
}
#endif /* ISL_CC1101_H_ */
