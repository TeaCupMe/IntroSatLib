//#ifndef DEVICE_H_
//#define DEVICE_H_
//
//#include <stdint.h>
//
//namespace IntroSatLib {
//class Device {
//	enum class DeviceState: uint8_t {
//		Default,
//		Initialized,
//		Error,
//		PowerDown
//	};
//
//	struct ErrorDescriptor {
//		uint8_t field1;
//		uint8_t field2;
////		iont
//	};
//
//	union Error {
//		ErrorDescriptor errorDescriptor;
//		uint16_t errorCode;
//	};
//
//private:
//	Error _error = {0};
//	DeviceState _state = DeviceState::Default;
//protected:
//	void setHALError(uint8_t error);
//
//	void setErrorFlags(uint8_t flags);
//	void setErrorFlagBit(uint8_t bit, uint8_t value);
//	void setErrorFlagBit(uint8_t bit);
//	void resetErrorFlagBit(uint8_t bit);
//
//	uint8_t getErrorFlags();
//	uint8_t getErrorFlagBit(uint8_t bit);
//
//	ErrorDescriptor getError();
//	void setState(Device::DeviceState state);
//public:
//	virtual uint8_t Init() = 0;
//	Device::DeviceState GetState();
//	Device::ErrorDescriptor GetError();
//
//};
//} /* namespace IntroSatLib */
//
//#endif /* DEVICE_H_ */
