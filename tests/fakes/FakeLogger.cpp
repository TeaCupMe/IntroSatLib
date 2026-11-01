/**
 * @file FakeLogger.cpp
 * @brief No-op LOGGER override so host tests stay quiet.
 */
#include "Logger.h"

void LOGGER(const char* /*text*/, uint8_t /*len*/)
{
}
