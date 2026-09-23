#ifndef INTROSAT_PLATFORM_PSM_DRIVER_H_
#define INTROSAT_PLATFORM_PSM_DRIVER_H_

#include "Device/UARTDevice.h"
#include "Platform/PSM/Commands.h"
#include "Platform/PSM/Types.h"
#include <stdint.h>

namespace IntroSatLib {

/**
 * \~russian UART-клиент платы питания pl_psm 1.x (115200 8N1).
 *
 * \~russian Каждая команда — 1 байт: (opcode & 0xF0) | (arg & 0x0F).
 * \~russian Канальные операции сами шлют SELECT_CHANNEL, затем команду.
 *
 * \~russian Пример:
 * \code
 * #include "Platform/PSM.h"
 * IntroSatLib::PSM psm(huart2);
 * psm.Init();
 * psm.SetPowerMode(PSM::PowerMode::Normal);
 * psm.EnableChannel(PSM::PowerChannel::Payload5v);
 * PSM::ChannelInfo info{};
 * psm.GetChannelInfo(PSM::PowerChannel::Payload5v, info);
 * \endcode
 */
class PSM : public UARTDevice {
public:
	using Cmd = platform::psm::commands::Cmd;
	using Ack = platform::psm::commands::Ack;
	using PowerChannel = platform::psm::types::PowerChannel;
	using PowerMode = platform::psm::types::PowerMode;
	using ChannelState = platform::psm::types::ChannelState;
	using ChannelInfo = platform::psm::types::ChannelInfo;
	using ExtendedChannelInfo = platform::psm::types::ExtendedChannelInfo;
	using HeaterMode = platform::psm::types::HeaterMode;
	using HeaterState = platform::psm::types::HeaterState;
	using HeaterInfo = platform::psm::types::HeaterInfo;

	static constexpr uint16_t ackTimeoutMs = 200;

	enum class OCPreset: uint8_t
	{
		Off,
		_100mA,
		_200mA,
		_300mA,
		_400mA,
		_500mA,
		_600mA,
		_750mA,
		_900mA,
		_1000mA,
		_1250mA,
		_1500mA,
		_1750mA,
		_2000mA,
		_2250mA,
		_2500mA
	};

	enum class UVPreset: uint8_t
	{
		Off,
		_95,
		_90,
		_85,
		_80,
		_75,
		_70,
		_65,
		_60,
		_55,
		_50,
		_45,
		_40,
		_35,
		_30,
		_25
	};


	explicit PSM(interfaces::UART uart) : UARTDevice(uart) {}

	ISL_StatusTypeDef Init() override;

	ISL_StatusTypeDef Ping();
	ISL_StatusTypeDef SetPowerMode(PowerMode mode);
	ISL_StatusTypeDef SaveConfig();

	ISL_StatusTypeDef SelectChannel(PowerChannel channel);
	ISL_StatusTypeDef EnableChannel(PowerChannel channel);
	ISL_StatusTypeDef DisableChannel(PowerChannel channel);
	ISL_StatusTypeDef GetChannelInfo(PowerChannel channel, ChannelInfo& info);
	ISL_StatusTypeDef GetExtendedChannelInfo(PowerChannel channel, ExtendedChannelInfo& info);

	/** \~russian preset: 0 = Off, 1..15 = kOvercurrentPresetAmps (макс. 2.5 А). */
	ISL_StatusTypeDef SetOvercurrent(PowerChannel channel, OCPreset preset);
	/** \~russian preset: 0 = Off, 1..15 = kUndervoltagePresetPercent. Только 3.3/5 В. */
	ISL_StatusTypeDef SetUndervoltage(PowerChannel channel, UVPreset preset);

	ISL_StatusTypeDef SetHeaterMode(HeaterMode mode);
	ISL_StatusTypeDef GetHeaterInfo(HeaterInfo& info);

	static constexpr bool isPowerChannel(PowerChannel channel)
	{
		return static_cast<uint8_t>(channel) < static_cast<uint8_t>(PowerChannel::Qty);
	}

	static constexpr bool channelSupportsUv(PowerChannel channel)
	{
		return channel == PowerChannel::Main3v3 ||
			channel == PowerChannel::Payload3v3 ||
			channel == PowerChannel::Main5v ||
			channel == PowerChannel::Payload5v;
	}

private:
	static constexpr uint8_t makeCmd(Cmd cmd, uint8_t arg = 0)
	{
		return static_cast<uint8_t>(cmd) | (arg & 0x0F);
	}

	ISL_StatusTypeDef transactAck(uint8_t command);
	ISL_StatusTypeDef transactPayload(uint8_t command, uint8_t* buf, uint16_t length);
	ISL_StatusTypeDef selectThen(PowerChannel channel, Cmd cmd, uint8_t arg = 0);
};

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

} /* namespace IntroSatLib */

#endif /* INTROSAT_PLATFORM_PSM_DRIVER_H_ */
