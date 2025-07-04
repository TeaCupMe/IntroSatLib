//#include "Device.h"
//
//void IntroSatLib::Device::setHALError(uint8_t error) {
//	_error.errorDescriptor.halStatusTypeDef = error;
//}
//
//void IntroSatLib::Device::setErrorFlags(uint8_t flags) {
//}
//
//void IntroSatLib::Device::setErrorFlagBit(uint8_t bit, uint8_t value) {
//	if (value) {
//		setFlagBit(bit);
//	} else {
//		resetFlagBit(bit);
//	}
//
//}
//
//void IntroSatLib::Device::setErrorFlagBit(uint8_t bit) {
//	if (bit > 7) bit = 7;
//	_error.errorDescriptor.errorFlags |= (1 << bit);
//}
//
//void IntroSatLib::Device::resetErrorFlagBit(uint8_t bit) {
//	setFlagBit(bit, 0);
//}
//
//IntroSatLib::Device::ErrorDescriptor IntroSatLib::Device::getError() {
//}
//
//uint8_t IntroSatLib::Device::GetErrorBit() {
//}
