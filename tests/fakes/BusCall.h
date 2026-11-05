/**
 * @file BusCall.h
 * @brief One recorded adapter call for CallLog (VIRTUAL host backend).
 */
#ifndef INTROSATLIB_TEST_BUS_CALL_H_
#define INTROSATLIB_TEST_BUS_CALL_H_

#include "BusOp.h"

#include <cstdint>
#include <vector>

namespace IntroSatLib {
namespace test {

struct BusCall {
	BusOp op{};
	uint8_t address = 0;
	uint8_t reg = 0;
	uint8_t nBytes = 0;
	uint32_t value = 0;
	std::vector<uint8_t> data;
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_BUS_CALL_H_ */
