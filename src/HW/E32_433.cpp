/*
 * E32_433.cpp
 *
 *  Created on: Mar 13, 2025
 *      Author: User
 */

#include "E32_433.h"

namespace IntroSatLib {

void E32_433::SetMode(MODE mode, uint8_t ignoreAUX) {
	if (mode!=MODE::NORMAL) _receiveEnable = false;
//	HAL_Delay(10);
	waitForReady();
	HAL_GPIO_WritePin(M0_Port, M0_Pin, (mode&0x1)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(M1_Port, M1_Pin, ((mode>>1)&0x1)?GPIO_PIN_SET:GPIO_PIN_RESET);
//	HAL_Delay(1);
//	while ((HAL_GPIO_ReadPin(AUX_Port, AUX_Pin) == GPIO_PIN_RESET) && ignoreAUX); // TODO add aux check
	if (!ignoreAUX)	waitForReady(100);
//	if (mode == MODE::NORMAL) _receiveEnable = true;

//	HAL_Delay(2); // TODO FIX wait >2 ms after AUX becomes high (as in datasheet)
}

void E32_433::SetSettings(E32_SettingsBytes* _sb) {

	SetMode(MODE::SLEEP, false);
	_writeSettings(_sb);
	SetMode(MODE::NORMAL, false);
	// TODO implement
}

void E32_433::SetUartParity(UART_PARITY parity) {
	// TODO implement
}

void E32_433::SetUARTBaudrate(UART_BAUDRATE baudrate) {
	// TODO implement
}

void E32_433::SetAirDatarate(AIR_DATARATE datarate) {
	SetMode(MODE::SLEEP, false);
	_readSettings();
	_settings.SPED &= ~(0b111);
	_settings.SPED |= datarate & (0b111);
	_writeSettings(&_settings);
	SetMode(MODE::NORMAL);
}

void E32_433::SetChannel(uint8_t channel) {
	SetMode(MODE::SLEEP, false);
	_readSettings();
	_settings.CHAN = (uint8_t) channel;
	_writeSettings(&_settings);
	SetMode(MODE::NORMAL);
	// TODO implement
}

void E32_433::SetFixedTransmission(TRANSMISSION_MODE mode) {
	// TODO implement
}

void E32_433::SetWakeUpTime(WAKE_UP_TIME wtime) {
	// TODO implement
}

void E32_433::SetFEC(FEC fec) {
	// TODO implement
}

void E32_433::SetTXPower(TX_POWER power) {
	SetMode(MODE::SLEEP, false);
	_readSettings();
	_settings.OPTION &= ~(0b11);
	_settings.OPTION |= ((uint8_t) power) & (0b11);
	_writeSettings(&_settings);
	SetMode(MODE::NORMAL);
}

void E32_433::SetIODriveMode(IO_DRIVE_MODE driveMode) {
	SetMode(MODE::SLEEP, false);
	_readSettings();
	_settings.OPTION &= ~(0b1 << 6);
	_settings.OPTION |= ((uint8_t) driveMode);
	_writeSettings(&_settings);
	SetMode(MODE::NORMAL);
}


ISL_StatusTypeDef E32_433::GetSettings(E32_SettingsBytes* settings) {
	ISL_StatusTypeDef status = ReadSettings();
	if (status != ISL_StatusTypeDef::ISL_OK) return status;
	memcpy((void *) settings, (void*) &_settings, 5);
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef E32_433::ReadSettings() {
	SetMode(MODE::SLEEP, 0);
//	uint8_t tx_buf[3] = {GET_PARAMETERS, GET_PARAMETERS, GET_PARAMETERS};
//	uint8_t rx_buf[1] = {0};
//	HAL_UART_Transmit(_uart, tx_buf, 3, 300);
//	HAL_UART_Receive(_uart, rx_buf, 1, 200);
//	if (rx_buf[0] != 0xC0) { // Check if module responds with 0xC0
//			return HAL_ERROR;
//		}
//	HAL_UART_Receive(_uart, (uint8_t*) &_settings, 5, 300);
	_readSettings();
	SetMode(MODE::NORMAL, 0);
	return ISL_StatusTypeDef::ISL_OK;
}

E32_433::E32_433(UART_HandleTypeDef* uart) {
	_uart = uart;


}

void E32_433::SetM0Pin(GPIO_TypeDef *m0_port, uint16_t m0_pin) {
	M0_Port = m0_port;
	M0_Pin = m0_pin;
}

void E32_433::SetM1Pin(GPIO_TypeDef *m1_port, uint16_t m1_pin) {
	M1_Port = m1_port;
	M1_Pin = m1_pin;
}

void E32_433::SetAUXPin(GPIO_TypeDef *aux_port, uint16_t aux_pin) {
	AUX_Port = aux_port;
	AUX_Pin = aux_pin;
}

void E32_433::Send(uint8_t *data, uint16_t len) {
	_receiveEnable = false;
	waitForReady();
	HAL_UART_Transmit(_uart, data, len, len*2);
	waitForLowAUX();
	waitForReady(5);
	_receiveEnable = true;

}

ISL_StatusTypeDef E32_433::Init() {
	waitForReady();
	HAL_Delay(20);
//	SetMode(MODE::SLEEP, false);
	ReadSettings();
//	SetMode(MODE::NORMAL, 0);
	waitForReady();
	return ISL_StatusTypeDef::ISL_OK;
}
void E32_433::DumpSettings(UART_HandleTypeDef* uart, E32_SettingsBytes sb) {
	char str[40] = {0};
	printToUart(uart, "LoRa configuration:\n\0");
	sprintf(str, "Channel: 0x%X\n", sb.CHAN);
	printToUart(uart, str);
	sprintf(str, "write_ptr: %d\n", _rx_write_offset);
	printToUart(uart, str);
	sprintf(str, "read_ptr: %d\n", _rx_read_offset);
	printToUart(uart, str);
//	sprintf(str, "Channel: 0x%X\n\r", sb.CHAN);
//	printToUart(uart, str);
}

HAL_StatusTypeDef E32_433::_readSettings(uint8_t retries) {
	uint8_t tx_buf[3] = {GET_PARAMETERS, GET_PARAMETERS, GET_PARAMETERS};
	uint8_t rx_buf[1] = {0};
	while (rx_buf[0] != 0xC0 && retries > 0) {
		HAL_UART_Transmit(_uart, tx_buf, 3, 300);
		HAL_UART_Receive(_uart, rx_buf, 1, 200);
	}
	if (rx_buf[0] != 0xC0) { // Check if module responds with 0xC0
		return HAL_ERROR;
	}
	HAL_UART_Receive(_uart, (uint8_t*) &_settings, 5, 300);
	return HAL_OK;
}

HAL_StatusTypeDef E32_433::_writeSettings(E32_SettingsBytes *_sb,
		uint8_t retries) {
	uint8_t tx_buf[1] = { E32_COMMAND::SET_PARAMETERS_SAVE };
	uint8_t rx_buf[6];
	waitForReady(5);
//	_readSettings();
//	waitForReady(5);
	HAL_UART_Transmit(_uart, tx_buf, 1, 200);
//	HAL_UART_Transmit(_uart, &_sb->CHAN,1,100);
	HAL_UART_Transmit(_uart, (uint8_t*) _sb, 5, 200);
	HAL_UART_Receive(_uart, rx_buf, 1, 1000);
	if (rx_buf[0] != 0xC0) { // Check if module responds with 0xC0
		return HAL_ERROR;
	}
	HAL_UART_Receive(_uart, (uint8_t*) &_settings, 5, 300);
	return HAL_OK;

}

void E32_433::Init(uint16_t rx_buf_size) {
	_rx_buf = new uint8_t[rx_buf_size];
	_rx_buf_size = rx_buf_size;
	Init();
}

void E32_433::SetExternalRxBuffer(uint8_t *rx_buf_ptr, uint16_t rx_buf_size) {
	_rx_buf = rx_buf_ptr;
	_rx_buf_size = rx_buf_size;
}

uint16_t E32_433::Receive() {
	if (!_receiveEnable) return 0;

	HAL_StatusTypeDef status = HAL_ERROR;
	uint16_t received = 0;
	bool packetBreak = false;
	if (_receiveAvailable && _receiveEnable) {
		_receiveEnable = false;
		do {
			status = HAL_UART_Receive(_uart, _rx_buf+_rx_write_offset, 1, packetBreak?700:20);
			if (status == HAL_OK) {
				_rx_write_offset = (_rx_write_offset + 1)%_rx_buf_size;
				received++;
				packetBreak = received%58 == 0;
			}
		}
		while( status == HAL_OK);
		_receiveEnable = true;
	}
	_receiveAvailable = false;

	return received;
}

void E32_433::GetReceivedData(uint8_t *external_buf_ptr, uint16_t len) {
	if (Available() < len) len = Available();
	for (int i = 0; i<len; i++) {
		external_buf_ptr[i] = _rx_buf[(_rx_read_offset+i)%_rx_buf_size];
	}
	_rx_read_offset = (_rx_read_offset + len) % _rx_buf_size;
}

bool E32_433::Available(uint16_t amount) {
	return (_rx_write_offset - _rx_read_offset > amount);
}

uint16_t E32_433::Available() {
	if (_rx_read_offset==_rx_write_offset) return 0;
	if (_rx_read_offset < _rx_write_offset) {
		return _rx_write_offset - _rx_read_offset;
	} else {
		return _rx_buf_size-_rx_read_offset + _rx_write_offset;
	}
}

void E32_433::SetReceive() {
	_receiveAvailable = true;
}
}




