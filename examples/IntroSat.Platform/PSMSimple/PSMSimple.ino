/* Подключение библиотек */
#include <ISL_Bootloader.h>
#include "Platform/PSM.h"

#include "HardwareSerial.h"
#include "Platform/PSM/Types.h"

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

/* Создаём объект для работы с аппаратным UART */
HardwareSerial psmSerial(PA1, PA0); // RX, TX

/* UART платы питания pl_psm 1.x: 115200 8N1
   TX хоста -> PA10 (USART1_RX) PSM
   RX хоста -> PA9  (USART1_TX) PSM
   общий GND */
PSM psm(psmSerial);

void setup() {
	Serial.begin(115200);
	psmSerial.begin(115200);

	Serial.println("Инициализация платы питания PSM");
	while (psm.Init()) {
		Serial.print(".");
		delay(200);
	}

	/* Normal: включение/выключение каналов по UART */
	psm.SetPowerMode(PSM::PowerMode::Normal);

	/* OC preset 9 = 1 А; UV preset 2 = 90% от номинала 5 В */
	psm.SetOvercurrent(PSM::PowerChannel::Payload5v, PSM::OCPreset::_1000mA);
	psm.SetUndervoltage(PSM::PowerChannel::Payload5v, PSM::UVPreset::_90);
	psm.EnableChannel(PSM::PowerChannel::Payload5v);
}

void loop() {
	char command = 0;
	if (Serial.available())
	{
		command = Serial.read();
	}

	if (command == 'b') {
		EnterBootloader();
	} 
	else if (command == '3')
	{
		ChannelInfo channelInfo;
		psm.GetChannelInfo(PSM::PowerChannel::Payload3v3, channelInfo);
		if   (channelInfo.state == ChannelState::On) psm.DisableChannel(PowerChannel::Payload3v3);
		else  psm.DisableChannel(PowerChannel::Payload3v3);
	} 
	else if (command == '5')
	{
		ChannelInfo channelInfo;
		psm.GetChannelInfo(PSM::PowerChannel::Payload5v, channelInfo);
		if   (channelInfo.state == ChannelState::On) psm.DisableChannel(PowerChannel::Payload5v);
		else  psm.DisableChannel(PowerChannel::Payload5v);
	} 

	for (uint8_t i = 0; i < static_cast<uint8_t>(PSM::PowerChannel::Qty); ++i) {
		const auto channel = static_cast<PSM::PowerChannel>(i);
		PSM::ChannelInfo info{};
		if (psm.GetChannelInfo(channel, info) != ISL_OK) {
			Serial.print(channelName(channel));
			Serial.println(": нет ответа");
			continue;
		}
		Serial.print(channelName(channel));
		Serial.print("  ");
		Serial.print(channelStateName(info.state));
		Serial.print("  V=");
		Serial.print(info.voltage, 3);
		Serial.print("  I=");
		Serial.println(info.current, 3);
	}
	Serial.println();
	delay(1000);
}
