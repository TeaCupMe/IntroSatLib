#pragma once

#include "Arduino.h"

#include <string>

class HardwareSerial {
public:
	std::string output;
	int available() { return 0; }
	int availableForWrite() { return 256; }
	void setTimeout(uint16_t) {}
	std::size_t readBytes(uint8_t*, std::size_t) { return 0; }
	std::size_t write(const uint8_t* data, std::size_t count) {
		output.append(reinterpret_cast<const char*>(data), count);
		return count;
	}
};
