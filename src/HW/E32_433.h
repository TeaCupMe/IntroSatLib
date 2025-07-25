/*
 * E32_433.h
 *
 *  Created on: Mar 11, 2025
 *      Author: User
 */

#ifndef E32_433_H_
#define E32_433_H_

#include "Device/RadioTransceiver.h"
#include "stdint.h"
#include <cstring>
#include "stdio.h"
#include "stm32h7xx_hal.h"

namespace IntroSatLib {
struct E32_SettingsBytes {
//			uint8_t CMD;
	uint16_t ADDR;
	uint8_t SPED;
	uint8_t CHAN;
	uint8_t OPTION;
};

class E32_433: public RadioTransceiver {
//private:
public: // TODO change to private
	GPIO_TypeDef *M0_Port;
	GPIO_TypeDef *M1_Port;
	GPIO_TypeDef *AUX_Port;
	uint16_t M0_Pin;
	uint16_t M1_Pin;
	uint16_t AUX_Pin;

	bool _receiveEnable;
	bool _receiveAvailable;

	uint8_t *_rx_buf;
	uint16_t _rx_buf_size;
	uint16_t _rx_write_offset = 0;
	uint16_t _rx_read_offset = 0;

	UART_HandleTypeDef *_uart;
	E32_SettingsBytes _settings;


	HAL_StatusTypeDef _readSettings(uint8_t retries = 3);
	HAL_StatusTypeDef _writeSettings(E32_SettingsBytes *_sb,
			uint8_t retries = 3);
	inline void waitForHighAUX(uint16_t timeout = 1000);
	inline void waitForLowAUX(uint16_t timeout = 1000);
	inline void waitForReady(uint32_t afterAuxTime = 4);
	inline void printToUart(UART_HandleTypeDef *uart, const char *str,
			uint16_t len = 0);
public:
	enum E32_COMMAND : uint8_t {
		SET_PARAMETERS_SAVE = 0xC0,
		GET_PARAMETERS = 0xC1,
		SET_PARAMETERS_NO_SAVE = 0xC2,
		GET_VERSION = 0xC3,
		SET_RESET = 0xC4
	};
	enum MODE : uint8_t {
		NORMAL = 0b00, WAKE_UP = 0b01, POWER_SAVING = 0b10, SLEEP = 0b11
	};
	enum UART_PARITY : uint8_t {
		PARITY_8N1 = 0b00 << 6, PARITY_8O1 = 0b01 << 6, PARITY_8E1 = 0b10 << 6
	};
	enum UART_BAUDRATE : uint8_t {
		BR_1200 = 0,
		BR_2400 = 0b001 << 3,
		BR_4800 = 0b010 << 3,
		BR_9600 = 0b011 << 3,
		BR_19200 = 0b100 << 3,
		BR_38400 = 0b101 << 3,
		BR_57600 = 0b110 << 3,
		BR_115200 = 0b111 << 3
	};
	enum AIR_DATARATE : uint8_t {
		DR_300 = 0,
		DR_1200 = 0b001,
		DR_2400 = 0b010,
		DR_4800 = 0b011,
		DR_9600 = 0b100,
		DR_19200 = 0b101
	};

	enum TRANSMISSION_MODE : uint8_t {
		M_TRANSPARENT = 0b0, M_FIXED = 0b1 << 7
	};

	enum IO_DRIVE_MODE : uint8_t {
		IO_OPENCOLLECTOR = 0b0, IO_PUSHPULL = 0b1 << 6
	};
	enum WAKE_UP_TIME : uint8_t {
		T_250ms = 0,
		T_500ms = 0b001 << 3,
		T_750ms = 0b010 << 3,
		T_1000ms = 0b011 << 3,
		T_1250ms = 0b100 << 3,
		T_1500ms = 0b101 << 3,
		T_1750ms = 0b110 << 3,
		T_2000ms = 0b111 << 3
	};

	enum FEC : uint8_t {
		FEC_DISABLED = 0, FEC_ENABLED = 0b1 << 2
	};

	enum class TX_POWER : uint8_t {
		P_33dBm, P_30dBm, P_27dBm, P_24dBm, P_21dBm, P_DEFAULT = 0b00
	};
	enum class TX_POWER_S : uint8_t {
		P_33dBm = 0b00, P_30dBm = 0b01, P_27dBm = 0b10, P_24dBm = 0b11
	};
	enum class TX_POWER_D : uint8_t {
		P_30dBm = 0b00, P_27dBm = 0b01, P_24dBm = 0b10, P_21dBm = 0b11
	};
//	typedef struct  {
//		uint8_t CMD = 0x00;
//		uint16_t ADDR = 0x0000;
//		uint8_t SPED = PARITY_8N1 | BR_9600 | DR_2400;
//		uint8_t CHAN = 0x17;
//		uint8_t OPTION = M_TRANSPARENT | IO_PUSHPULL | T_250ms | FEC_ENABLED | (uint8_t) TX_POWER::P_DEFAULT;
//	} SettingsBytes;

	MODE _mode = MODE::NORMAL;
	bool Available(uint16_t amount);
	uint16_t Available();
	void SetReceive();
	uint16_t Receive();
	void GetReceivedData(uint8_t* external_buf_ptr, uint16_t len);
	ISL_StatusTypeDef Init();
	void Init(uint16_t rx_buf_size);
	void SetExternalRxBuffer(uint8_t *rx_buf_ptr, uint16_t rx_buf_size);
	void SetM0Pin(GPIO_TypeDef *m0_port, uint16_t m0_pin);
	void SetM1Pin(GPIO_TypeDef *m1_port, uint16_t m1_pin);
	void SetAUXPin(GPIO_TypeDef *aux_port, uint16_t aux_pin);
	void SetMode(MODE mode = MODE::NORMAL, uint8_t ignoreAUX = false);
	void SetSettings(E32_SettingsBytes *sb);
	void SetUartParity(UART_PARITY parity = UART_PARITY::PARITY_8N1);
	void SetUARTBaudrate(UART_BAUDRATE baudrate = UART_BAUDRATE::BR_9600);
	void SetAirDatarate(AIR_DATARATE datarate = AIR_DATARATE::DR_2400);
	void SetChannel(uint8_t channel);
	void SetFixedTransmission(TRANSMISSION_MODE mode =
			TRANSMISSION_MODE::M_TRANSPARENT);
	void SetWakeUpTime(WAKE_UP_TIME wtime = WAKE_UP_TIME::T_250ms);
	void SetFEC(FEC fec = FEC::FEC_ENABLED);
	void SetTXPower(TX_POWER power = TX_POWER::P_DEFAULT);
	void SetIODriveMode(IO_DRIVE_MODE driveMode);
	void Send(uint8_t *data, uint16_t len) override;
	ISL_StatusTypeDef ReadSettings();
	ISL_StatusTypeDef GetSettings(E32_SettingsBytes *settings);
	void DumpSettings(UART_HandleTypeDef *uart, E32_SettingsBytes sb);
	E32_433(UART_HandleTypeDef *uart);
};
inline void E32_433::waitForHighAUX(uint16_t timeout) {

	uint32_t timeoutTime = HAL_GetTick() + timeout;
	while ((HAL_GPIO_ReadPin(AUX_Port, AUX_Pin) != GPIO_PIN_SET)
			&& (HAL_GetTick() < timeoutTime))
		;
	return;
}
inline void E32_433::waitForReady(uint32_t afterAuxTime) {
	uint32_t startTime = HAL_GetTick();
	while (HAL_GetTick() - afterAuxTime < startTime) {
		if (HAL_GPIO_ReadPin(AUX_Port, AUX_Pin) == GPIO_PIN_RESET) {
			waitForHighAUX();
			startTime = HAL_GetTick();
		}
	}
}

inline void E32_433::printToUart(UART_HandleTypeDef *uart, const char *str,
		uint16_t len) {
	HAL_UART_Transmit(uart, (uint8_t*) str, len ? len : strlen(str), 200);
}
inline void E32_433::waitForLowAUX(uint16_t timeout) {
	uint32_t timeoutTime = HAL_GetTick() + timeout;
	while ((HAL_GPIO_ReadPin(AUX_Port, AUX_Pin) != GPIO_PIN_RESET)
			&& (HAL_GetTick() < timeoutTime))
		;
	return;
}


}

#endif /* E32_433_H_ */
