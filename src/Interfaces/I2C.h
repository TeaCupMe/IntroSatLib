#ifndef ADAPTER_I2C_I2C_H_
#define ADAPTER_I2C_I2C_H_

#include "IntroSatLib_def.h"

#include <stdint.h>

namespace IntroSatLib {
namespace interfaces {

/**
 * Public I2C bus API used by drivers and tests.
 * Platform backends implement this as I2CAdapter (library-internal).
 */
class I2C {
public:
	virtual ~I2C() = default;

	virtual ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0) = 0;

	virtual ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef readMem(uint8_t deviceAddress,
									  uint8_t reg,
									  uint8_t* data,
									  uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes) = 0;

	virtual ISL_StatusTypeDef writeMem(uint8_t deviceAddress,
									   uint8_t reg,
									   uint8_t* data,
									   uint8_t nBytes) = 0;
};

} /* namespace interfaces */
} /* namespace IntroSatLib */

#endif /* ADAPTER_I2C_I2C_H_ */
