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

#define ISL_INTERNAL
#include "Adapter/System.h"
#include "Adapter/SPI.h"
#include "Adapter/GPIO.h"

#if defined(ISL_SPI_ENABLED) and defined(ISL_GPIO_ENABLED)

#include "math.h"
#include "Device/CC1101/CC1101.h"
#include "Device/CC1101/CC1101_Consts.h"

namespace IntroSatLib {
using namespace std;
using namespace cc1101_consts;

CC1101::Status CC1101::Init(Modulation mod, double freq, double drate) {
	Status status;
	Deselect();

	hardReset();
	system::Delay(10);

	uint8_t partnum = getChipPartNumber();
	uint8_t version = getChipVersion();
	if (partnum != CC1101_PARTNUM
			|| (version != CC1101_VERSION && version != CC1101_VERSION_LEGACY)) {
		return STATUS_CHIP_NOT_FOUND;
	}

	setRegs();

	setModulation(mod);

	if ((status = setFrequency(freq)) != STATUS_OK) {
		return status;
	}

	if ((status = setDataRate(drate)) != STATUS_OK) {
		return status;
	}

	setOutputPower(0);

	setState(STATE_IDLE);
	flushRxBuffer();
	flushTxBuffer();

	return STATUS_OK;
}

void CC1101::setRegs() {
	/* Automatically calibrate when going from IDLE to RX or TX. */
	writeRegField(ConfigRegister::MCSM0, 1, 5, 4);

	/* Enable append status */
	writeRegField(ConfigRegister::PKTCTRL1, 1, 2, 2);

	/* Disable data whitening. */
	setDataWhitening(false);
}

void CC1101::setModulation(Modulation mod) {
	this->mod = mod;
	writeRegField(ConfigRegister::MDMCFG2, (uint8_t) mod, 6, 4);

	setOutputPower(this->power);

	if (mod == Modulation::M_MSK || mod == Modulation::M_4FSK) {
		setManchester(false);
	}
}

CC1101::Status CC1101::setFrequency(double freq) {
	if (!((freq >= 300.0 && freq <= 348.0) || (freq >= 387.0 && freq <= 464.0)
			|| (freq >= 779.0 && freq <= 928.0))) {
		return STATUS_INVALID_PARAM;
	}

	this->freq = freq;
	setState(STATE_IDLE);

	uint32_t f = ((freq * 65536.0) / CC1101_CRYSTAL_FREQ);
	writeReg(ConfigRegister::FREQ0, f & 0xff);
	writeReg(ConfigRegister::FREQ1, (f >> 8) & 0xff);
	writeReg(ConfigRegister::FREQ2, (f >> 16) & 0xff);

	setOutputPower(this->power);

	return STATUS_OK;
}

CC1101::Status CC1101::setFrequencyDeviation(double dev) {
	double xosc = CC1101_CRYSTAL_FREQ * 1000;

	uint32_t devMin = (xosc / ((uint32_t) 1 << 17)) * (8 + 0) * 1;
	uint32_t devMax = (xosc / ((uint32_t) 1 << 17)) * (8 + 7) * (1 << 7);

	if (dev < devMin || dev > devMax) {
		return STATUS_INVALID_PARAM;
	}

	uint8_t bestE = 0, bestM = 0;
	double diff = devMax;

	for (uint8_t e = 0; e <= 7; e++) {
		for (uint8_t m = 0; m <= 7; m++) {
			double t = (xosc / (double) ((uint32_t) 1 << 17)) * (8 + m)
					* (double) ((uint32_t) 1 << e);
			if (fabs(dev - t) < diff) {
				diff = fabs(dev - t);
				bestE = e;
				bestM = m;
			}
		}
	}

	writeRegField(ConfigRegister::DEVIATN, bestM, 2, 0);
	writeRegField(ConfigRegister::DEVIATN, bestE, 6, 4);

	return STATUS_OK;
}

void CC1101::setChannel(uint8_t ch) {
	writeReg(ConfigRegister::CHANNR, ch);
}

CC1101::Status CC1101::setChannelSpacing(double sp) {
	double xosc = CC1101_CRYSTAL_FREQ * 1000;

	uint32_t spMin = (xosc / (double) ((uint32_t) 1 << 18)) * (256. + 0.) * 1.;
	uint32_t spMax = (xosc / (double) ((uint32_t) 1 << 18)) * (256. + 255.)
			* 8.;

	if (sp < spMin || sp > spMax) {
		return STATUS_INVALID_PARAM;
	}

	uint8_t bestE = 0, bestM = 0;
	double diff = spMax;

	for (uint8_t e = 0; e <= 3; e++) {
		for (uint16_t m = 0; m <= 255; m++) {
			double t = (xosc / (double) ((uint32_t) 1 << 18)) * (256. + m)
					* (double) ((uint32_t) 1 << e);
			if (fabs(sp - t) < diff) {
				diff = fabs(sp - t);
				bestE = e;
				bestM = m;
			}
		}
	}

	writeReg(ConfigRegister::MDMCFG0, bestM);
	writeRegField(ConfigRegister::MDMCFG1, bestE, 1, 0);

	return STATUS_OK;
}

CC1101::Status CC1101::setDataRate(double drate) {

	static const double range[][2] =
	{
		[0] = { 0.6, 500.0 }, /* Modulation::M_2FSK: 0.6 - 500 kBaud */
		[1] = { 0.6, 250.0 }, /* Modulation::M_GFSK */
		[2] = { 0.0, 0.0 },   /* gap */
		[3] = { 0.6, 250.0 }, /* Modulation::M_ASK_OOK */
		[4] = { 0.6, 300.0 }, /* Modulation::M_4FSK */
		[5] = { 0.0, 0.0 },   /* gap */
		[6] = { 0.0, 0.0 },   /* gap */
		[7] = { 26.0, 500.0 } /* Modulation::M_MSK */
	};

	if (drate < range[static_cast<uint32_t>(mod)][0] || drate > range[static_cast<uint32_t>(mod)][1]) {
		return STATUS_INVALID_PARAM;
	}

	this->drate = drate;

	uint32_t xosc = CC1101_CRYSTAL_FREQ * 1000;
	uint8_t e = log2((drate * (double) ((uint32_t) 1 << 20)) / xosc);
	uint32_t m = round(
			drate * ((double) ((uint32_t) 1 << (28 - e)) / xosc) - 256.);

	if (m == 256) {
		m = 0;
		e++;
	}

	writeRegField(ConfigRegister::MDMCFG4, e, 3, 0);
	writeReg(ConfigRegister::MDMCFG3, (uint8_t) m);

	return STATUS_OK;
}

CC1101::Status CC1101::setRxBandwidth(double bw) {
	/*
	 CC1101 supports the following channel filter bandwidths [kHz]:
	 (assuming a 26 MHz crystal).

	 \ E  0     1     2     3
	 M +----------------------
	 0 | 812 | 406 | 203 | 102
	 1 | 650 | 335 | 162 |  81
	 2 | 541 | 270 | 135 |  68
	 3 | 464 | 232 | 116 |  58

	 */

	uint32_t bwMin = (CC1101_CRYSTAL_FREQ * 1000) / (8 * (4 + 3) * (1 << 3));
	uint32_t bwMax = (CC1101_CRYSTAL_FREQ * 1000) / (8 * (4 + 0) * (1 << 0));

	if (bw < bwMin || bw > bwMax) {
		return STATUS_INVALID_PARAM;
	}

	uint8_t bestE = 0, bestM = 0;
	double diff = bwMax;

	for (uint8_t e = 0; e <= 3; e++) {
		for (uint8_t m = 0; m <= 3; m++) {
			double t = (double) (CC1101_CRYSTAL_FREQ * 1000)
					/ (8 * (4 + m) * (1 << e));
			if (fabs(bw - t) < diff) {
				diff = fabs(bw - t);
				bestE = e;
				bestM = m;
			}
		}
	}

	writeRegField(ConfigRegister::MDMCFG4, bestE, 7, 6);
	writeRegField(ConfigRegister::MDMCFG4, bestM, 5, 4);

	return STATUS_OK;
}

void CC1101::setOutputPower(int8_t power) {

	static const uint8_t powers[][8] = { [0 /* 315 Mhz */] = { 0x12, 0x0d, 0x1c,
			0x34, 0x51, 0x85, 0xcb, 0xc2 }, [1 /* 433 Mhz */] = { 0x12, 0x0e,
			0x1d, 0x34, 0x60, 0x84, 0xc8, 0xc0 }, [2 /* 868 Mhz */] = { 0x03,
			0x0f, 0x1e, 0x27, 0x50, 0x81, 0xcb, 0xc2 }, [3 /* 915 MHz */] = {
			0x03, 0x0e, 0x1e, 0x27, 0x8e, 0xcd, 0xc7, 0xc0 } };

	uint8_t powerIdx, freqIdx;

	if (freq <= 348.0) {
		freqIdx = 0;
	} else if (freq <= 464.0) {
		freqIdx = 1;
	} else if (freq <= 891.5) {
		freqIdx = 2;
	} else {
		freqIdx = 3;
	}

	if (power <= -30) {
		powerIdx = 0;
	} else if (power <= -20) {
		powerIdx = 1;
	} else if (power <= -15) {
		powerIdx = 2;
	} else if (power <= -10) {
		powerIdx = 3;
	} else if (power <= 0) {
		powerIdx = 4;
	} else if (power <= 5) {
		powerIdx = 5;
	} else if (power <= 7) {
		powerIdx = 6;
	} else {
		powerIdx = 7;
	}

	this->power = power;

	if (mod == Modulation::M_ASK_OOK) {
		/* No shaping. Use only the first 2 entries in the power table. */
		uint8_t data[2] = { 0x00, powers[freqIdx][powerIdx] };
		writeRegBurst(ConfigRegister::PATABLE, data, sizeof(data));
		writeRegField(ConfigRegister::FREND0, 1, 2, 0); /* PA_POWER = 1 */
	} else {
		writeReg(ConfigRegister::PATABLE, powers[freqIdx][powerIdx]);
		writeRegField(ConfigRegister::FREND0, 0, 2, 0); /* PA_POWER = 0 */
	}
}

void CC1101::setPreambleLength(PreambleLength length) {
	writeRegField(ConfigRegister::MDMCFG1, static_cast<uint8_t>(length), 6, 4);
}

void CC1101::setSyncWord(uint16_t sync) {
	writeReg(ConfigRegister::SYNC1, sync >> 8);
	writeReg(ConfigRegister::SYNC0, sync & 0xff);
}

void CC1101::setSyncMode(SyncMode mode) {
	writeRegField(ConfigRegister::MDMCFG2, static_cast<uint8_t>(mode), 2, 0);
}

void CC1101::setPacketLengthMode(PacketLengthMode mode, uint8_t length) {
	this->pktLenMode = mode;
	this->pktLen = length;

	writeRegField(ConfigRegister::PKTCTRL0, static_cast<uint8_t>(mode), 1, 0);

	switch (mode) {
	case PacketLengthMode::FIXED:
		writeReg(ConfigRegister::PKTLEN, length);
		break;
	case PacketLengthMode::VARIABLE:
		/* Indicates the maximum packet length allowed. */
		writeReg(ConfigRegister::PKTLEN, length);
		break;
	}
}

void CC1101::setAddressFilteringMode(AddressFilteringMode mode) {
	this->addrFilterMode = mode;

	writeRegField(ConfigRegister::PKTCTRL1, static_cast<uint8_t>(mode), 1, 0);
}

void CC1101::setCrc(bool enable) {
	writeRegField(ConfigRegister::PKTCTRL0, (uint8_t) enable, 2, 2);
}

void CC1101::setDataWhitening(bool enable) {
	writeRegField(ConfigRegister::PKTCTRL0, (uint8_t) enable, 6, 6);
}

CC1101::Status CC1101::setManchester(bool enable) {
	if (enable &&
			(this->mod == Modulation::M_MSK ||
			 this->mod == Modulation::M_4FSK ||
			 this->fec))
	{
		return STATUS_BAD_STATE;
	}

	this->manchester = enable;
	writeRegField(ConfigRegister::MDMCFG2, (uint8_t) enable, 3, 3);
	return STATUS_OK;
}

CC1101::Status CC1101::setFEC(bool enable) {
	if (enable
			&& (this->pktLenMode != PacketLengthMode::FIXED || this->manchester)) {
		return STATUS_BAD_STATE;
	}

	this->fec = enable;
	writeRegField(ConfigRegister::MDMCFG1, (uint8_t) enable, 7, 7);
	return STATUS_OK;
}

int8_t CC1101::getRSSI() {
	if (this->rssi >= 128) {
		return (((int8_t) this->rssi - 256) / 2) - 74;
	} else {
		return ((int8_t) this->rssi / 2) - 74;
	}
}

uint8_t CC1101::getLQI() {
	return this->lqi;
}

CC1101::Status CC1101::transmit(uint8_t *data, size_t length, uint8_t addr) {
	uint8_t bytesSent = 0, dataSent = 0;
	size_t curPktLen = length;
	Status ret = STATUS_OK;

	if (addrFilterMode != AddressFilteringMode::NONE) {
		curPktLen++;
	}

	if (curPktLen > 255) {
		return STATUS_LENGTH_TOO_BIG;
	}

	setState(STATE_IDLE);
	flushTxBuffer();

	switch (pktLenMode) {
	case PacketLengthMode::FIXED:
		if (curPktLen < this->pktLen) {
			return STATUS_LENGTH_TOO_SMALL;
		}

		if (curPktLen > this->pktLen) {
			return STATUS_LENGTH_TOO_BIG;
		}
		break;
	case PacketLengthMode::VARIABLE:
		writeReg(ConfigRegister::FIFO, (uint8_t) curPktLen);
		bytesSent++;
		break;
	}

	if (addrFilterMode != AddressFilteringMode::NONE) {
		writeReg(ConfigRegister::FIFO, addr);
		bytesSent++;
	}

	uint8_t l = min((uint8_t) length, (uint8_t) (CC1101_FIFO_SIZE - bytesSent));
	writeRegBurst(ConfigRegister::FIFO, data, l);
	bytesSent += l;
	dataSent += l;

	setState(STATE_TX);

	while (dataSent < length) {
		uint8_t bytesInFifo = readRegField(ConfigRegister::TXBYTES, 6, 0);

		if (bytesInFifo < CC1101_FIFO_SIZE) {
			uint8_t bytesToWrite = min((uint8_t) (length - dataSent),
					(uint8_t) (CC1101_FIFO_SIZE - bytesInFifo));
			writeRegBurst(ConfigRegister::FIFO, data + dataSent, bytesToWrite);
			bytesSent += bytesToWrite;
			dataSent += bytesToWrite;
		}

		if (currentState == STATE_TXFIFO_UNDERFLOW) {
			ret = STATUS_TXFIFO_UNDERFLOW;
			flushTxBuffer();
			break;
		}

//    yield();
	}

	while (getState() != STATE_IDLE) {
		if (currentState == STATE_TXFIFO_UNDERFLOW) {
			ret = STATUS_TXFIFO_UNDERFLOW;
			flushTxBuffer();
		}
		system::Delay(1);
//    delayMicroseconds(50);
//    yield();
	}

	return ret;
}

CC1101::Status CC1101::receive(uint8_t *data, size_t length, size_t *read,
		uint8_t addr) {
	if (length > 255) {
		return STATUS_LENGTH_TOO_BIG;
	}

	uint8_t bytesInFifo, bytesRead = 0, curPktLen = 0;
	Status ret = STATUS_OK;

	writeReg(ConfigRegister::ADDR, addr);

	setState(STATE_IDLE);
	flushRxBuffer();
	setState(STATE_RX);

	switch (pktLenMode) {
	case PacketLengthMode::FIXED:
		curPktLen = this->pktLen;
		break;
	case PacketLengthMode::VARIABLE:
		waitForBytesInFifo();
		curPktLen = readReg(ConfigRegister::FIFO);
		bytesRead++;
		break;
	}

	uint8_t dataRead = 0, dataLength = curPktLen;

	if (addrFilterMode != AddressFilteringMode::NONE) {
		waitForBytesInFifo();
		(void) readReg(ConfigRegister::FIFO);
		bytesRead++;
		dataLength--;
	}

	if (dataLength > length) {
		setState(STATE_IDLE);
		return STATUS_LENGTH_TOO_SMALL;
	}

	/*
	 For packet lengths less than 64 bytes it is recommended to wait until
	 the complete packet has been received before reading it out of the RX FIFO.
	 */
	if (dataLength <= (uint8_t) (CC1101_FIFO_SIZE - bytesRead)) {
		do {
			system::Delay(1);
//      delayMicroseconds(15);
//      yield();
			bytesInFifo = waitForBytesInFifo();
		} while (bytesInFifo < dataLength);
	}

	while (dataRead < dataLength) {
		bytesInFifo = waitForBytesInFifo();
		uint8_t bytesToRead = min((uint8_t) (dataLength - dataRead),
				bytesInFifo);
		readRegBurst(ConfigRegister::FIFO, data + dataRead, bytesToRead);
		bytesRead += bytesToRead;
		dataRead += bytesToRead;

		if (currentState == STATE_RXFIFO_OVERFLOW) {
			ret = STATUS_RXFIFO_OVERFLOW;
			flushRxBuffer();
			break;
		}
	}

	while (getState() != STATE_IDLE) {
		if (currentState == STATE_RXFIFO_OVERFLOW) {
			ret = STATUS_RXFIFO_OVERFLOW;
			flushRxBuffer();
			break;
		}
		system::Delay(1);
//    delayMicroseconds(50);
//    yield();
	}

	if (ret != STATUS_OK) {
		return ret;
	}

	this->rssi = readReg(ConfigRegister::FIFO);
	uint8_t v = readReg(ConfigRegister::FIFO);
	this->lqi = v & 0x7f;

	flushRxBuffer();

	bool crc_ok = (v >> 7) & 1;
	if (!crc_ok) {
		ret = STATUS_CRC_MISMATCH;
	}

	if (read != nullptr) {
		*read = dataLength;
	}

	return ret;
}

uint8_t CC1101::waitForBytesInFifo() {
	uint8_t bytesInFifo = readRegField(ConfigRegister::RXBYTES, 6, 0);
	while (bytesInFifo == 0) {
		system::Delay(1);
//    delayMicroseconds(15);
//    yield();
		bytesInFifo = readRegField(ConfigRegister::RXBYTES, 6, 0);
	}
	return bytesInFifo;
}

void CC1101::receiveCallback(void (*func)(void)) {
	/*
	 Associated to the RX FIFO: Asserts when RX FIFO is filled at or above
	 the RX FIFO threshold or the end of packet is reached. De-asserts when
	 the RX FIFO is empty.
	 */
	writeRegField(ConfigRegister::IOCFG0, 1, 5, 0);

	// TODO: Move to other method.
	flushRxBuffer();
	setState(STATE_RX);

	recvCallback = true;
//  attachInterrupt(digitalPinToInterrupt(gd0), func, RISING);
}

CC1101::State CC1101::getState() {
	sendCmd(CC1101_CMD_NOP);
	return currentState;
}

void CC1101::setState(State state) {
	switch (state) {
	case STATE_IDLE:
		sendCmd(CC1101_CMD_IDLE);
		break;
	case STATE_TX:
		sendCmd(CC1101_CMD_TX);
		break;
	case STATE_RX:
		sendCmd(CC1101_CMD_RX);
		break;
	default:
		/* Not supported. */
		return;
	}

	while (getState() != state) {
//    delayMicroseconds(100);
		system::Delay(1);
	}
}

void CC1101::setGDIOConfig(uint8_t Gdio, GdioConfig config) {

	// Only GDIO0/1/2 are available
	switch (Gdio)
	{
		case 0:
			writeRegField(ConfigRegister::IOCFG0, static_cast<uint8_t>(config), 5, 0);
			break;
		case 1:
			writeRegField(ConfigRegister::IOCFG1, static_cast<uint8_t>(config), 5, 0);
			break;
		case 2:
			writeRegField(ConfigRegister::IOCFG2, static_cast<uint8_t>(config), 5, 0);
			break;
		default:
			break;
	}
}

void CC1101::saveStatus(uint8_t status) {
	currentState = (State) ((status >> 4) & 0b111);
}

void CC1101::hardReset() {
	Deselect();
	system::Delay(1);
//  delayMicroseconds(5);
	Select();
	system::Delay(1);
//  delayMicroseconds(5);
	Deselect();
	system::Delay(1);
//  delayMicroseconds(40);

	Select();
	waitReady();

	TransmitByteSPI(CC1101_CMD_RES);
	// delayMicroseconds(40);
	system::Delay(1);
	waitReady();
	Deselect();
}

void CC1101::flushRxBuffer() {
	if (currentState != STATE_IDLE && currentState != STATE_RXFIFO_OVERFLOW) {
		return;
	}
	sendCmd(CC1101_CMD_FRX);
}

void CC1101::flushTxBuffer() {
	if (currentState != STATE_IDLE && currentState != STATE_TXFIFO_UNDERFLOW) {
		return;
	}
	sendCmd(CC1101_CMD_FTX);
}

uint8_t CC1101::getChipPartNumber() {
	return readReg(ConfigRegister::PARTNUM);
}

uint8_t CC1101::getChipVersion() {
	return readReg(ConfigRegister::VERSION);
}

uint8_t CC1101::readRegField(ConfigRegister reg, uint8_t hi, uint8_t lo) {
	return (readReg(reg) >> lo) & ((1 << (hi - lo + 1)) - 1);
}

uint8_t CC1101::readReg(ConfigRegister reg) {
	uint8_t addr = static_cast<uint8_t>(reg);

	uint8_t header = CC1101_CMD_TYPE_READ | (addr & 0b111111);

	if (addr >= static_cast<uint8_t>(ConfigRegister::PARTNUM)
				&& addr <= static_cast<uint8_t>(ConfigRegister::RCCTRL0_STATUS))
	{
		/* Status registers - access with the burst bit on. */
		header |= CC1101_CMD_TYPE_BURST;
	}

	Select();
	waitReady();

	saveStatus(TransmitByteSPI(header));
	uint8_t data;
	ReceiveSPI(&data, 1);

	Deselect();
	return data;
}

void CC1101::readRegBurst(ConfigRegister reg, uint8_t *buff, size_t size) {
	uint8_t addr = static_cast<uint8_t>(reg);

	if (addr >= static_cast<uint8_t>(ConfigRegister::PARTNUM)
			&& addr <= static_cast<uint8_t>(ConfigRegister::RCCTRL0_STATUS)) {
		/* Status registers are read-only. */
		return;
	}

	uint8_t header = CC1101_CMD_TYPE_READ | CC1101_CMD_TYPE_BURST | (addr & 0b111111);


	Select();
	waitReady();

	uint8_t resp;
	TransmitReceiveSPI(&header, &resp, 1);
	saveStatus(resp);

	ReceiveSPI(buff, size);

	Deselect();
}

void CC1101::writeRegField(ConfigRegister reg, uint8_t data, uint8_t hi, uint8_t lo) {
	data <<= lo;
	uint8_t current = readReg(reg);
	uint8_t mask = ((1 << (hi - lo + 1)) - 1) << lo;
	data = (current & ~mask) | (data & mask);
	writeReg(reg, data);
}

void CC1101::writeReg(ConfigRegister reg, uint8_t data) {
	uint8_t addr = static_cast<uint8_t>(reg);
	if (addr >= static_cast<uint8_t>(ConfigRegister::PARTNUM)
			&& addr <= static_cast<uint8_t>(ConfigRegister::RCCTRL0_STATUS)) {
		/* Status registers are read-only. */
		return;
	}

	uint8_t header = CC1101_CMD_TYPE_WRITE | (addr & 0b111111);

	Select();
	waitReady();

	uint8_t resp;
	TransmitReceiveSPI(&header, &resp, 1);
	saveStatus(resp);

	TransmitReceiveSPI(&data, &resp, 1);
	saveStatus(resp);

	Deselect();
}

void CC1101::writeRegBurst(ConfigRegister startReg, uint8_t *data, size_t size) {
	uint8_t addr = static_cast<uint8_t>(startReg);
	if (addr >= static_cast<uint8_t>(ConfigRegister::PARTNUM)
			&& addr <= static_cast<uint8_t>(ConfigRegister::RCCTRL0_STATUS)) {
		/* Status registers are read-only. */
		return;
	}

	uint8_t header = CC1101_CMD_TYPE_WRITE | CC1101_CMD_TYPE_BURST | (addr & 0b111111);

	Select();
	waitReady();

	uint8_t resp;
	TransmitReceiveSPI(&header, &resp, 1);
	saveStatus(resp);

	// Maybe optimize out this loop?
	for (size_t i = 0; i < size; i++) {
		TransmitReceiveSPI(data + i, &resp, 1);
		saveStatus(resp);
	}

	Deselect();
}

void CC1101::sendCmd(uint8_t cmd) {
	uint8_t header = CC1101_CMD_TYPE_WRITE | (cmd & 0b111111);

	Select();
	waitReady();

	uint8_t resp;
	TransmitReceiveSPI(&header, &resp, 1);
	saveStatus(resp);

	Deselect();
}

void CC1101::waitReady() {
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3)
  // ESP32C3/S3 does not allow a pin to be polled whilst it is attached to the
  // SPI peripheral. This will hang forever.
  // Fortunately, the CC1101 datasheet (pp29-30) states that MISO immediately
  // goes low on CS unless in a low power mode. As this library does not (yet)
  // support low power modes, we can safely return immediately.
  return;
  #endif
//  while (digitalRead(MISO))
//    ;
	return;
}

void CC1101::setAddress(uint8_t address) {
	writeReg(ConfigRegister::ADDR, address);
}

} /* namespace IntroSatLib */

#endif /* ISL_SPI_ENABLED */

