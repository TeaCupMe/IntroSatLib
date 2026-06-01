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
#include "Device/CC1101/CC1101_Consts.h"

namespace IntroSatLib {

using namespace cc1101_consts;

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
		STATE_IDLE = 0, 		/* IDLE state */
		STATE_RX, 				/* Receive mode */
		STATE_TX, 				/* Transmit mode */
		STATE_FSTXON, 			/* Fast TX ready */
		STATE_CALIBRATE, 		/* Freq synthesizer calibration is running */
		STATE_SETTLIN, 			/* PLL is settling */
		STATE_RXFIFO_OVERFLOW, 	/* RX FIFO has overflowed */
		STATE_TXFIFO_UNDERFLOW, /* TX FIFO has underflowed */
	};

	enum class Modulation : uint8_t {
		M_2FSK 	= 0,
		M_GFSK 	= 1,
		M_ASK_OOK = 3,
		M_4FSK 	= 4,
		M_MSK 	= 7
	};

	enum class SyncMode: uint8_t {
		NO_PREAMBLE = 0, 	/* No preamble/sync */
		PREAMBULE_15_16, 	/* 15/16 sync word bits detected */
		PREAMBULE_16_16, 	/* 16/16 sync word bits detected */
		PREAMBULE_30_32, 	/* 30/32 sync word bits detected */
		NO_PREAMBLE_CS, 	/* No preamble/sync, CS above threshold */
		PREAMBULE_15_16_CS, /* 15/16 + carrier-sense above threshold */
		PREAMBULE_16_16_CS, /* 16/16 + carrier-sense above threshold */
		PREAMBULE_30_32_CS, /* 30/32 + carrier-sense above threshold */
	};

	enum class PacketLengthMode : uint8_t {
		FIXED = 0, /* Length configured in PKTLEN register */
		VARIABLE,  /* Packet length put in the first byte */
	// TODO: PKT_LEN_MODE_INFINITE = 2,  /* Infinite packet length mode */
	};

	enum class PreambleLength : uint8_t {
		TWO_BYTES = 0,
		THREE_BYTES,
		FOUR_BYTES,
		SIX_BYTES,
		EIGHT_BYTES,
		TWELVE_BYTES,
		TWENTY_FOUR_BYTES
	};

	enum class AddressFilteringMode : uint8_t {
		NONE = 0, 			/* No address check */
		CHECK = 1, 			/* Address check, no broadcast */
		CHECK_BC_0 = 2, 	/* Address check, 0 broadcast */
		CHECK_BC_0_255 = 3 	/* Address check, 0 and 255 broadcast */
	};

	CC1101(interfaces::SPI _spi, interfaces::GPIO csPin): SPIDevice(_spi, csPin) {}
	CC1101(interfaces::SPI _spi, interfaces::GPIO_HANDLE_TYPE csPin): SPIDevice(_spi, csPin) {}


	Status Init(Modulation mod, double freq, double drate);

	ISL_StatusTypeDef Init() override {
		return Init(Modulation::M_ASK_OOK, 433.5, 4) == Status::STATUS_OK ? ISL_StatusTypeDef::ISL_OK : ISL_StatusTypeDef::ISL_ERROR;
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
	void setAddress(uint8_t address);
	void setAddressFilteringMode(AddressFilteringMode mode);
	void setPacketLengthMode(PacketLengthMode mode, uint8_t length = 255);
	void setSyncMode(SyncMode mode);
	void setPreambleLength(PreambleLength length);
	void setSyncWord(uint16_t sync);

	Status transmit(uint8_t *data, size_t length, uint8_t addr = 0);
	Status transmit(const uint8_t *data, size_t length, uint8_t addr = 0)
	{
		return transmit(const_cast<uint8_t*>(data), length, addr);
	}

	Status receive(uint8_t *data, size_t length, size_t *read = nullptr,
			uint8_t addr = 0);
	int8_t getRSSI();
	uint8_t getLQI();

	void receiveCallback(void (*func)(void));
	void setGDIOConfig(uint8_t Gdio, GdioConfig config);

private:
	void chipSelect();
	void chipDeselect();
	void waitReady();
	uint8_t waitForBytesInFifo();

	uint8_t readRegField(ConfigRegister reg, uint8_t hi, uint8_t lo);
	uint8_t readReg(ConfigRegister reg);
	void readRegBurst(ConfigRegister reg, uint8_t *buff, size_t size);

	void writeRegField(ConfigRegister reg, uint8_t data, uint8_t hi, uint8_t lo);
	void writeReg(ConfigRegister reg, uint8_t data);
	void writeRegBurst(ConfigRegister reg, uint8_t *data, size_t size);

	void sendCmd(uint8_t addr);

	void setRegs();
	void hardReset();
	void flushRxBuffer();
	void flushTxBuffer();

	State getState();
	void setState(State);
	void saveStatus(uint8_t status);

//	uint8_t gd0, gd2;
//	uint8_t clk, miso, mosi;

	State currentState = STATE_IDLE;
	Modulation mod = Modulation::M_2FSK;
	PacketLengthMode pktLenMode = PacketLengthMode::FIXED;
	AddressFilteringMode addrFilterMode = AddressFilteringMode::NONE;
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
