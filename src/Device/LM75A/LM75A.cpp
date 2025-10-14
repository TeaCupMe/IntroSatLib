/*
 * LM75A.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#include "LM75A.h"

namespace IntroSatLib {

ISL_StatusTypeDef LM75A::Init() {
	return IsReady();
}

void LM75A::PowerDown(bool shutdown) {
	UNUSED(shutdown);
	//TODO implement
}

LM75A::LM75A(const interfaces::I2C &i2c, uint8_t address) :
		I2CDevice(new interfaces::I2C(i2c), address) {
}


uint16_t LM75A::GetRawTemperature() {
	uint8_t raw[2];
	ReadRegisterI2C(RegisterMap::TEMPERATURE, raw, 2);
	int16_t data = raw[0] << 8 | raw[1];
	return data;
}

float LM75A::GetTemperature() {
	return ((float) GetRawTemperature()) / 256.0;
}

uint16_t LM75A::GetTemperatureTimes8() {
	return GetRawTemperature() / 32;
}

uint8_t LM75A::GetConfig() {
	uint8_t data = 0;
	ReadRegisterI2C(RegisterMap::CONFIGURATION, &data, 1);
	return data;
}


//void LM75A::DumpData(UART_HandleTypeDef* _uart) {
//	char tx_buf[40] = {0};
//	sprintf(tx_buf, "lm75ad init: %d\n\r", Init());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	sprintf(tx_buf, "Raw temperature: %X\n\r", GetRawTemperature());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	sprintf(tx_buf, "Temperature: %f\n\r",  GetTemperature());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	sprintf(tx_buf, "Temperature in F: %f\n\r",  GetTemperatureF());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	sprintf(tx_buf, "Temperature times 8: %d\n\r",  GetTemperatureTimes8());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	sprintf(tx_buf, "Config: %X\n\r",  GetConfig());
//	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
////	HAL_Delay(1000);
//}

//void LM75A::DumpData(std::function<void(uint8_t*, uint16_t)> transmitMethod) {
//	char tx_buf[40] = {0};
//	sprintf(tx_buf, "lm75ad init: %d\n\r", Init());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Raw temperature: %X\n\r", GetRawTemperature());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature: %f\n\r",  GetTemperature());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature in F: %f\n\r",  GetTemperatureF());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature times 8: %d\n\r",  GetTemperatureTimes8());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Config: %X\n\r",  GetConfig());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
////	HAL_Delay(1000);
//}


}
