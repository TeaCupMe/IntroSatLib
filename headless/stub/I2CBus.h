#ifndef HEADLESS_STUB_I2CBUS_H_
#define HEADLESS_STUB_I2CBUS_H_

/**
 * Local stub matching DI design signatures from
 * docs/di-virtual-adapters-design.md.
 *
 * Owned temporarily by the headless MVP until rebasing onto the Driver test
 * MVP branch that lands IntroSatLib::interfaces::I2CBus in src/Adapter/.
 * Do not treat this as the library's DI seam.
 */

#include "IntroSatLib_def.h"
#include <stdint.h>

namespace IntroSatLib {
namespace interfaces {

class I2CBus {
public:
	virtual ~I2CBus() = default;

	virtual ISL_StatusTypeDef isReady(uint8_t deviceAddress,
					  uint8_t waitIsReady = 0) = 0;
	virtual ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data,
				       uint8_t nBytes) = 0;
	virtual ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg,
					  uint8_t* data, uint8_t nBytes) = 0;
	virtual ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data,
					uint8_t nBytes) = 0;
	virtual ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg,
					   uint8_t* data, uint8_t nBytes) = 0;
};

} /* namespace interfaces */
} /* namespace IntroSatLib */

#endif /* HEADLESS_STUB_I2CBUS_H_ */
