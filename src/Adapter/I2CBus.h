/**
 * @file I2CBus.h
 * @brief Abstract I2C bus interface (no platform headers).
 *
 * Host unit tests and FakeI2CBus depend on this header only.
 * Concrete IntroSatLib::interfaces::I2C implements the platform backend.
 */
#ifndef ADAPTER_I2CBUS_H_
#define ADAPTER_I2CBUS_H_

#include "IntroSatLib_def.h"

#include <stdint.h>

namespace IntroSatLib {
namespace interfaces {

class I2CBus {
public:
	virtual ~I2CBus() = default;

	virtual ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0) = 0;

	virtual ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef readMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef writeMem(
		uint8_t deviceAddress,
		uint8_t reg,
		uint8_t* data,
		uint8_t nBytes) = 0;
};

} /* namespace interfaces */
} /* namespace IntroSatLib */

#endif /* ADAPTER_I2CBUS_H_ */
