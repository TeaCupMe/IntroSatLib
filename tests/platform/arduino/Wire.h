#pragma once

#include "Arduino.h"

// Minimal Wire surface for exercising the real Arduino I2C adapter on the host.
class TwoWire {
public:
	unsigned probes = 0;
	uint8_t lastAddress = 0;
	uint8_t result = 0;

	void beginTransmission(uint8_t address) { ++probes; lastAddress = address; }
	uint8_t endTransmission() { return result; }
	std::size_t write(uint8_t) { return 1; }
	std::size_t write(const uint8_t*, std::size_t count) { return count; }
	int available() { return 0; }
	int read() { return -1; }
	std::size_t readBytes(uint8_t*, std::size_t) { return 0; }
	uint8_t requestFrom(uint8_t, uint8_t) { return 0; }
	uint8_t requestFrom(uint8_t, uint8_t, uint8_t, uint8_t, bool) { return 0; }
};
