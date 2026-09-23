/* Подключение библиотек */
#include <IS_Bluetooth.h>
#include "Platform/PSM.h"

#include "HardwareSerial.h"

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

HardwareSerial psmSerial(PA3, PA2);

/* UART платы питания pl_psm 1.x: 115200 8N1
   TX хоста -> PA10 (USART1_RX) PSM
   RX хоста -> PA9  (USART1_TX) PSM
   общий GND */
PSM psm(psmSerial);

static const char* channelName(PSM::PowerChannel channel)
{
	switch (channel) {
		case PSM::PowerChannel::Battery:        return "Battery";
		case PSM::PowerChannel::PayloadBattery: return "PayloadBattery";
		case PSM::PowerChannel::Main3v3:        return "Main3v3";
		case PSM::PowerChannel::Payload3v3:     return "Payload3v3";
		case PSM::PowerChannel::Main5v:         return "Main5v";
		case PSM::PowerChannel::Payload5v:      return "Payload5v";
		default:                                return "?";
	}
}

static const char* channelStateName(PSM::ChannelState state)
{
	switch (state) {
		case PSM::ChannelState::Off:             return "Off";
		case PSM::ChannelState::On:              return "On";
		case PSM::ChannelState::OffOvercurrent:  return "OffOC";
		case PSM::ChannelState::OffUndervoltage: return "OffUV";
		default:                                 return "?";
	}
}

static void checkBootloader()
{
	if (Serial.available() && Serial.read() == 'b') {
		enter_bootloader();
	}
}

void setup() {
	Serial.begin(115200);
	psmSerial.begin(115200);

	Serial.println("Инициализация платы питания PSM");
	while (psm.Init()) {
		Serial.print(".");
		delay(200);
	}
	Serial.println("\nPSM на связи");

	/* Normal: включение/выключение каналов по UART */
	psm.SetPowerMode(PSM::PowerMode::Normal);

	/* OC preset 9 = 1 А; UV preset 2 = 90% от номинала 5 В */
	psm.SetOvercurrent(PSM::PowerChannel::Payload5v, PSM::OCPreset::_1000mA);
	psm.SetUndervoltage(PSM::PowerChannel::Payload5v, PSM::UVPreset::_90);
	psm.EnableChannel(PSM::PowerChannel::Payload5v);
}

void loop() {
	checkBootloader();

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
