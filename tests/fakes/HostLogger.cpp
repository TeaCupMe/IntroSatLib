/**
 * @file HostLogger.cpp
 * @brief Strong LOGGER for host unit tests — writes to stderr.
 *
 * MockI2C call logging uses IntroSatLib logText/logHEX/logStatus; those only
 * emit when LOGGER is defined. Default tests stay quiet because MockI2C logs
 * only when setLogging(true) or INTROSATLIB_MOCK_I2C_LOG is set.
 */
#include "Logger.h"

#include <cstdio>

void LOGGER(const char* text, uint8_t len)
{
	if (text == nullptr || len == 0) {
		return;
	}
	std::fwrite(text, 1, len, stderr);
	std::fflush(stderr);
}
