#ifndef INTROSATLIB_TEST_MOCK_I2C_HPP_
#define INTROSATLIB_TEST_MOCK_I2C_HPP_

#include "Adapter/I2C/I2C.h"

#include <gmock/gmock.h>

namespace IntroSatLib {
namespace test {

// Use StrictMock<MockI2C> when every bus transaction must be accounted for.
class MockI2C : public interfaces::I2C {
public:
	MOCK_METHOD(ISL_StatusTypeDef, isReady, (uint8_t deviceAddress, uint8_t waitIsReady), (override));
	MOCK_METHOD(ISL_StatusTypeDef, read, (uint8_t deviceAddress, uint8_t* data, uint8_t nBytes), (override));
	MOCK_METHOD(ISL_StatusTypeDef, readMem,
		(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes), (override));
	MOCK_METHOD(ISL_StatusTypeDef, write, (uint8_t deviceAddress, uint8_t* data, uint8_t nBytes), (override));
	MOCK_METHOD(ISL_StatusTypeDef, writeMem,
		(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes), (override));
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_MOCK_I2C_HPP_ */
