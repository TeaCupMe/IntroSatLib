#include "Platform/PSM/Types.h"
#define ISL_INTERNAL

#include "Adapter/UART.h"
#if defined(ISL_UART_ENABLED)

#include "Platform/PSM/PSM.h"
#include <string.h>

namespace IntroSatLib {

namespace {
constexpr uint8_t kAck = static_cast<uint8_t>(platform::psm::commands::Ack::Ack);
constexpr uint8_t kNack = static_cast<uint8_t>(platform::psm::commands::Ack::Nack);
}

ISL_StatusTypeDef PSM::Init()
{
	return Ping();
}

ISL_StatusTypeDef PSM::Ping()
{
	return transactAck(makeCmd(Cmd::Ping));
}

ISL_StatusTypeDef PSM::SetPowerMode(PowerMode mode)
{
	return transactAck(makeCmd(Cmd::SetPowerMode, static_cast<uint8_t>(mode)));
}

ISL_StatusTypeDef PSM::SaveConfig()
{
	return transactAck(makeCmd(Cmd::SafeCfg));
}

ISL_StatusTypeDef PSM::SelectChannel(PowerChannel channel)
{
	if (!isPowerChannel(channel))
	{
		return ISL_ERROR;
	}
	return transactAck(makeCmd(Cmd::SelectChannel, static_cast<uint8_t>(channel)));
}

ISL_StatusTypeDef PSM::EnableChannel(PowerChannel channel)
{
	return selectThen(channel, Cmd::EnableChannel);
}

ISL_StatusTypeDef PSM::DisableChannel(PowerChannel channel)
{
	return selectThen(channel, Cmd::DisableChannel);
}

ISL_StatusTypeDef PSM::GetChannelInfo(PowerChannel channel, ChannelInfo& info)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SelectChannel(channel));
	uint8_t raw[sizeof(ChannelInfo)];
	RETURN_STATUS_IF_NOT_OK_SILENT(transactPayload(makeCmd(Cmd::GetChannelInfo), raw, sizeof(raw)));
	memcpy(&info, raw, sizeof(info));
	return ISL_OK;
}

ISL_StatusTypeDef PSM::GetExtendedChannelInfo(PowerChannel channel, ExtendedChannelInfo& info)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SelectChannel(channel));
	uint8_t raw[sizeof(ExtendedChannelInfo)];
	RETURN_STATUS_IF_NOT_OK_SILENT(transactPayload(makeCmd(Cmd::GetExtChannelInfo), raw, sizeof(raw)));
	memcpy(&info, raw, sizeof(info));
	return ISL_OK;
}

ISL_StatusTypeDef PSM::SetOvercurrent(PowerChannel channel, OCPreset preset)
{
	if (static_cast<size_t>(preset) >= platform::psm::types::kProtectionPresetCount)
	{
		return ISL_ERROR;
	}
	return selectThen(channel, Cmd::SetOC, platform::psm::types::kOvercurrentPresetAmps[static_cast<size_t>(preset)]);
}

ISL_StatusTypeDef PSM::SetUndervoltage(PowerChannel channel, UVPreset preset)
{
	if (static_cast<size_t>(preset) >= platform::psm::types::kProtectionPresetCount || !channelSupportsUv(channel))
	{
		return ISL_ERROR;
	}
	return selectThen(channel, Cmd::SetUV, platform::psm::types::kUndervoltagePresetPercent[static_cast<size_t>(preset)]);
}

ISL_StatusTypeDef PSM::SetHeaterMode(HeaterMode mode)
{
	return transactAck(makeCmd(Cmd::SetHeaterMode, static_cast<uint8_t>(mode)));
}

ISL_StatusTypeDef PSM::GetHeaterInfo(HeaterInfo& info)
{
	uint8_t raw[sizeof(HeaterInfo)];
	RETURN_STATUS_IF_NOT_OK_SILENT(transactPayload(makeCmd(Cmd::GetHeaterInfo), raw, sizeof(raw)));
	memcpy(&info, raw, sizeof(info));
	return ISL_OK;
}

ISL_StatusTypeDef PSM::transactAck(uint8_t command)
{
	FlushRX();
	uint8_t tx = command;
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(&tx, 1, ackTimeoutMs));

	uint8_t rx = 0;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(&rx, 1, ackTimeoutMs));
	if (rx == kAck)
	{
		return ISL_OK;
	}
	return ISL_ERROR;
}

ISL_StatusTypeDef PSM::transactPayload(uint8_t command, uint8_t* buf, uint16_t length)
{
	if (buf == nullptr || length == 0)
	{
		return ISL_ERROR;
	}

	FlushRX();
	uint8_t tx = command;
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteUART(&tx, 1, ackTimeoutMs));

	RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(buf, 1, ackTimeoutMs));
	if (buf[0] == kNack)
	{
		return ISL_ERROR;
	}
	if (length > 1)
	{
		RETURN_STATUS_IF_NOT_OK_SILENT(ReadUART(buf + 1, static_cast<uint16_t>(length - 1), ackTimeoutMs));
	}
	return ISL_OK;
}

ISL_StatusTypeDef PSM::selectThen(PowerChannel channel, Cmd cmd, uint8_t arg)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SelectChannel(channel));
	return transactAck(makeCmd(cmd, arg));
}

} /* namespace IntroSatLib */

#endif /* ISL_UART_ENABLED */
