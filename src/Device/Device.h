#ifndef DEVICE_H_
#define DEVICE_H_

namespace IntroSatLib {
class Device {
	enum class DeviceState: uint8_t {
		Default,
		Initialized,
		Error,
		PowerDown
	};

	struct ErrorDescriptor {
		uint8_t Error;
		uint8_t origin;
		uint16_t flags;
	};

	union Error {
		ErrorDescriptor errorDescriptor;
		uint32_t errorCode;
	};

private:
	Error error = 0;
	DeviceState state = DeviceState::Default;
protected:
	void setHALError(uint8_t error);
	void setOrigin(uint8_t origin);
	void setFlags(uint32_t flags);
	void setFlagBit(uint8_t bit, uint8_t value);

	void setFlagBit(uint8_t bit);
	void resetFlagBit(uint8_t bit);

public:
	ErrorDescriptor GetError();
	uint8_t GetErrorBit();
};
} /* namespace IntroSatLib */

#endif /* DEVICE_H_ */
