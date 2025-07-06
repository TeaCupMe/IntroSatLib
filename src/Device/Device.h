#ifndef DEVICE_H_
#define DEVICE_H_

//#include <stdint.h>

namespace IntroSatLib {
class Device {
	enum class DeviceState: uint8_t {
		Default,
		Initialized,
		Error,
		PowerDown
	};

private:
	DeviceState _state = DeviceState::Default;

protected:
	void setState(Device::DeviceState state);

public:
	virtual ISL_StatusTypeDef Init() = 0;
	Device::DeviceState GetState() {
		return _state;
	};
};
} /* namespace IntroSatLib */

#endif /* DEVICE_H_ */
