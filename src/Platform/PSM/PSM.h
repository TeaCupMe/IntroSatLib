#ifndef INTROSAT_PLATFORM_PSM_DRIVER_H_
#define INTROSAT_PLATFORM_PSM_DRIVER_H_

#include "Device/UARTDevice.h"
#include "Platform/PSM/Commands.h"
#include "Platform/PSM/Types.h"
#include <stdint.h>

namespace IntroSatLib {

/**
 * \~russian @brief Драйвер платы питания pl_psm 1.x (115200 8N1).
 * \~english @brief Power supply board driver pl_psm 1.x (115200 8N1).
 * 
 * \~english Example:
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

	/**
	 * \~english @brief Enum for overcurrent presets.
	 * \~russian @brief Перечисление для предустановок перегрузки по току.
	 */
	enum class OCPreset: uint8_t
	{
		Off,		/**< \~english Off 	   \~russian Отключено */
		_100mA,		/**< \~english 100 mA  \~russian 100 мА    */
		_200mA,		/**< \~english 200 mA  \~russian 200 мА    */
		_300mA,		/**< \~english 300 mA  \~russian 300 мА    */
		_400mA,		/**< \~english 400 mA  \~russian 400 мА    */
		_500mA,		/**< \~english 500 mA  \~russian 500 мА    */
		_600mA,		/**< \~english 600 mA  \~russian 600 мА    */
		_750mA,		/**< \~english 750 mA  \~russian 750 мА    */
		_900mA,		/**< \~english 900 mA  \~russian 900 мА    */
		_1000mA,	/**< \~english 1000 mA \~russian 1000 мА   */
		_1250mA,	/**< \~english 1250 mA \~russian 1250 мА   */
		_1500mA,	/**< \~english 1500 mA \~russian 1500 мА   */
		_1750mA,	/**< \~english 1750 mA \~russian 1750 мА   */
		_2000mA,	/**< \~english 2000 mA \~russian 2000 мА   */
		_2250mA,	/**< \~english 2250 mA \~russian 2250 мА   */
		_2500mA		/**< \~english 2500 mA \~russian 2500 мА   */
	};

	/**
	 * \~english @brief Enum for undervoltage presets.
	 * \~russian @brief Перечисление для предустановок пониженного напряжения.
	 */
	enum class UVPreset: uint8_t
	{
		Off,	/**< \~english Off 	 \~russian Отключено */
		_95,	/**< \~english 95 %  \~russian 95 %    	 */
		_90,	/**< \~english 90 %  \~russian 90 %    	 */
		_85,	/**< \~english 85 %  \~russian 85 %    	 */
		_80,	/**< \~english 80 %  \~russian 80 %    	 */
		_75,	/**< \~english 75 %  \~russian 75 %    	 */
		_70,	/**< \~english 70 %  \~russian 70 %    	 */
		_65,	/**< \~english 65 %  \~russian 65 %    	 */
		_60,	/**< \~english 60 %  \~russian 60 %    	 */
		_55,	/**< \~english 55 %  \~russian 55 %    	 */
		_50,	/**< \~english 50 %  \~russian 50 %    	 */
		_45,	/**< \~english 45 %  \~russian 45 %    	 */
		_40,	/**< \~english 40 %  \~russian 40 %    	 */
		_35,	/**< \~english 35 %  \~russian 35 %    	 */
		_30,	/**< \~english 30 %  \~russian 30 %    	 */
		_25		/**< \~english 25 %  \~russian 25 %    	 */
	};

	/**
	 * \~english @brief Construct a new PSM object
	 * \~russian @brief Конструктор нового объекта PSM
	 * 
	 * \~english @param uart - The UART interface to use.
	 * \~russian @param uart - Интерфейс UART для использования.
	 */
	explicit PSM(interfaces::UART uart) : UARTDevice(uart) {}

	/**
	 * \~english @brief Initialize the PSM device.
	 * \~russian @brief Инициализация устройства PSM.
	 * 
	 * \~english @return * ISL_StatusTypeDef - Status of the initialization operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции инициализации.
	 */
	ISL_StatusTypeDef Init() override;

	/**
	 * \~english @brief Ping the PSM device to check if it is responsive.
	 * \~russian @brief Проверка связи с устройством PSM.
	 * 
	 * \~english @return * ISL_StatusTypeDef - Status of the ping operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции проверки связи.
	 */
	ISL_StatusTypeDef Ping();

	/**
	 * \~english @brief Set the Power Mode of the PSM device.
	 * \~russian @brief Установить режим питания устройства PSM.
	 * 
	 * \~english @param mode - The desired Power Mode to set.
	 * \~russian @param mode - Желаемый режим питания для установки.
	 * \~english @return * ISL_StatusTypeDef 
	 * \~russian @return * ISL_StatusTypeDef 
	 */
	ISL_StatusTypeDef SetPowerMode(PowerMode mode);

	/**
	 * \~english @brief Save the configuration of the PSM device.
	 * \~russian @brief Сохранить конфигурацию устройства PSM.
	 * 
	 * \~english @return * ISL_StatusTypeDef - Status of the save operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции сохранения.
	 */
	ISL_StatusTypeDef SaveConfig();

	/**
	 * \~english @brief Enable a power channel.
	 * \~russian @brief Включить канал питания.
	 * 
	 * \~english @param channel - The power channel to enable.
	 * \~russian @param channel - Канал питания для включения.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef EnableChannel(PowerChannel channel);

	/**
	 * \~english @brief Disable a power channel.
	 * \~russian @brief Отключить канал питания.
	 * 
	 * \~english @param channel - The power channel to disable.
	 * \~russian @param channel - Канал питания для отключения.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef DisableChannel(PowerChannel channel);

	/**
	 * \~english @brief Get power channel info.
	 * \~russian @brief Получить информацию о канале питания.
	 * 
	 * \~english @param channel - The power channel to get info for.
	 * \~russian @param channel - Канал питания для получения информации.
	 * \~english @param info - Reference to the ChannelInfo object to fill.
	 * \~russian @param info - Ссылка на объект ChannelInfo для заполнения.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef GetChannelInfo(PowerChannel channel, ChannelInfo& info);

	/**
	 * \~english @brief Get extended power channel info.
	 * \~russian @brief Получить расширенную информацию о канале питания.
	 * 
	 * \~english @param channel - The power channel to get extended info for.
	 * \~russian @param channel - Канал питания для получения расширенной информации.
	 * \~english @param info - Reference to the ExtendedChannelInfo object to fill.
	 * \~russian @param info - Ссылка на объект ExtendedChannelInfo для заполнения.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef GetExtendedChannelInfo(PowerChannel channel, ExtendedChannelInfo& info);

	/**
	 * \~english @brief Set the overcurrent preset for a power channel.
	 * \~russian @brief Установить предустановленное значение перегрузки для канала питания.
	 * 
	 * \~english @param channel - The power channel to set the preset for.
	 * \~russian @param channel - Канал питания для установки предустановленного значения.
	 * \~english @param preset - The overcurrent preset to set.
	 * \~russian @param preset - Предустановленное значение перегрузки для установки.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef SetOvercurrent(PowerChannel channel, OCPreset preset);
	/**
	 * \~english @brief Set the undervoltage preset for a power channel.
	 * \~russian @brief Установить предустановленное значение недонапряжения для канала питания.
	 * 
	 * \~english @param channel - The power channel to set the preset for.
	 * \~russian @param channel - Канал питания для установки предустановленного значения.
	 * \~english @param preset - The undervoltage preset to set.
	 * \~russian @param preset - Предустановленное значение недонапряжения для установки.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef SetUndervoltage(PowerChannel channel, UVPreset preset);

	ISL_StatusTypeDef SetHeaterMode(HeaterMode mode);
	ISL_StatusTypeDef GetHeaterInfo(HeaterInfo& info);

	/**
	 * \~english @brief Check if a channel is a valid power channel.
	 * \~russian @brief Проверка, является ли канал допустимым каналом питания.
	 * 
	 * \~english @param channel - The power channel to check.
	 * \~russian @param channel - Канал питания для проверки.
	 * \~english @return true - If the channel is valid.
	 * \~russian @return true - Если канал допустим.
	 * \~english @return false - If the channel is not valid.
	 * \~russian @return false - Если канал недопустим.
	 */
	static constexpr bool isPowerChannel(PowerChannel channel)
	{
		return static_cast<uint8_t>(channel) < static_cast<uint8_t>(PowerChannel::Qty);
	}

private:
	/**
	 * \~english @brief Check if a channel supports undervoltage protection.
	 * \~russian @brief Проверить, поддерживает ли канал защиту от недонапряжения.
	 * 
	 * \~english @param channel - The power channel to check.
	 * \~russian @param channel - Канал питания для проверки.
	 * \~english @return true - If the channel supports undervoltage protection.
	 * \~russian @return true - Если канал поддерживает защиту от недонапряжения.
	 * \~english @return false - If the channel does not support undervoltage protection.
	 * \~russian @return false - Если канал не поддерживает защиту от недонапряжения.
	 */
	static constexpr bool channelSupportsUv(PowerChannel channel)
	{
		return channel == PowerChannel::Main3v3 ||
			channel == PowerChannel::Payload3v3 ||
			channel == PowerChannel::Main5v ||
			channel == PowerChannel::Payload5v;
	}

	/**
	 * \~russian @brief Выбрать канал питания для дальнейшей работы.
	 * \~english @brief Select a power channel.
	 * 
	 * \~english @param channel - The power channel to select.
	 * \~russian @param channel - Канал питания для выбора.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef SelectChannel(PowerChannel channel);

	static constexpr uint8_t makeCmd(Cmd cmd, uint8_t arg = 0)
	{
		return static_cast<uint8_t>(cmd) | (arg & 0x0F);
	}

	/**
	 * \~english @brief Perform a transaction with an acknowledgment.
	 * \~russian @brief Выполнить транзакцию с получением подтверждения.
	 * 
	 * \~english @param command - The command to send.
	 * \~russian @param command - Команда для отправки.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef transactAck(uint8_t command);

	/**
	 * \~english @brief Perform a transaction with a payload.
	 * \~russian @brief Выполнить транзакцию с получением данных.
	 * 
	 * \~english @param command - The command to send.
	 * \~russian @param command - Команда для отправки.
	 * \~english @param buf - The buffer containing the payload.
	 * \~russian @param buf - Буфер, содержащий полезную нагрузку.
	 * \~english @param length - The length of the payload.
	 * \~russian @param length - Длина полезной нагрузки.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef transactPayload(uint8_t command, uint8_t* buf, uint16_t length);

	/**
	 * \~english @brief Select a power channel and then execute a command.
	 * \~russian @brief Выбрать канал питания и затем выполнить команду.
	 * 
	 * \~english @param channel - The power channel to select.
	 * \~russian @param channel - Канал питания для выбора.
	 * \~english @param cmd - The command to execute.
	 * \~russian @param cmd - Команда для выполнения.
	 * \~english @param arg - The argument for the command.
	 * \~russian @param arg - Аргумент для команды.
	 * \~english @return * ISL_StatusTypeDef - Status of the operation.
	 * \~russian @return * ISL_StatusTypeDef - Статус операции.
	 */
	ISL_StatusTypeDef selectThen(PowerChannel channel, Cmd cmd, uint8_t arg = 0);
	static constexpr uint16_t ackTimeoutMs = 200;
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
