#include "HeadlessLog.h"
#include "TcpI2CBus.h"

#include <cstdint>
#include <iostream>
#include <string>

namespace {

constexpr uint8_t kLm75aAddr = 0x48;
constexpr uint8_t kRegTemp = 0x00;
constexpr uint16_t kDefaultPort = 17375;

/**
 * Thin LM75A-shaped client over I2CBus (same register map as IntroSatLib::LM75A).
 * Can switch to real LM75A(I2CBus&) once that overload is used in this demo.
 */
class ThinLm75aClient {
public:
	ThinLm75aClient(IntroSatLib::interfaces::I2CBus& bus, uint8_t addr = kLm75aAddr)
		: bus_(bus), addr_(addr) {}

	ISL_StatusTypeDef init() {
		return bus_.isReady(addr_);
	}

	ISL_StatusTypeDef readRawTemperature(int16_t& out) {
		uint8_t raw[2] = {};
		const ISL_StatusTypeDef st =
			bus_.readMem(addr_, kRegTemp, raw, 2);
		if (st != ISL_OK) {
			return st;
		}
		out = static_cast<int16_t>((raw[0] << 8) | raw[1]);
		return ISL_OK;
	}

	ISL_StatusTypeDef readTemperatureC(float& out) {
		int16_t raw = 0;
		const ISL_StatusTypeDef st = readRawTemperature(raw);
		if (st != ISL_OK) {
			return st;
		}
		out = raw / 256.0f;
		return ISL_OK;
	}

private:
	IntroSatLib::interfaces::I2CBus& bus_;
	uint8_t addr_;
};

void usage(const char* argv0) {
	std::cerr << "Usage: " << argv0
		  << " [--host HOST] [--port PORT] [--addr HEX]\n"
		  << "        [-v|-vv|--verbose] [--log-level off|error|info|debug]\n"
		  << "  Env: INTROSATLIB_HEADLESS_LOG=off|error|info|debug\n";
}

} // namespace

int main(int argc, char** argv) {
	using IntroSatLib::headless::LogLevel;
	using IntroSatLib::headless::logInit;
	using IntroSatLib::headless::logMsg;

	std::string host = "127.0.0.1";
	uint16_t port = kDefaultPort;
	uint8_t addr = kLm75aAddr;
	LogLevel cliLevel = LogLevel::Off;
	bool cliLogSet = false;

	for (int i = 1; i < argc; ++i) {
		const std::string arg = argv[i];
		if (arg == "--host" && i + 1 < argc) {
			host = argv[++i];
		} else if (arg == "--port" && i + 1 < argc) {
			port = static_cast<uint16_t>(std::stoi(argv[++i]));
		} else if (arg == "--addr" && i + 1 < argc) {
			addr = static_cast<uint8_t>(std::stoul(argv[++i], nullptr, 0));
		} else if (arg == "-v" || arg == "--verbose") {
			cliLevel = LogLevel::Info;
			cliLogSet = true;
		} else if (arg == "-vv") {
			cliLevel = LogLevel::Debug;
			cliLogSet = true;
		} else if (arg == "--log-level" && i + 1 < argc) {
			cliLevel = IntroSatLib::headless::parseLogLevel(argv[++i]);
			cliLogSet = true;
		} else if (arg == "--help" || arg == "-h") {
			usage(argv[0]);
			return 0;
		} else {
			usage(argv[0]);
			return 1;
		}
	}

	logInit("demo", cliLevel, cliLogSet);

	IntroSatLib::headless::TcpI2CBus bus(host, port);
	if (!bus.connect()) {
		logMsg(LogLevel::Error, "connect",
		       "host=" + host + " port=" + std::to_string(port) + " ok=0");
		std::cerr << "demo: connect to " << host << ":" << port << " failed\n";
		return 1;
	}
	logMsg(LogLevel::Info, "connect",
	       "host=" + host + " port=" + std::to_string(port) + " ok=1");

	ThinLm75aClient sensor(bus, addr);
	const ISL_StatusTypeDef ready = sensor.init();
	if (ready != ISL_OK) {
		std::cerr << "demo: isReady failed status=" << static_cast<int>(ready)
			  << "\n";
		return 1;
	}

	int16_t raw = 0;
	float tempC = 0.f;
	if (sensor.readRawTemperature(raw) != ISL_OK ||
	    sensor.readTemperatureC(tempC) != ISL_OK) {
		std::cerr << "demo: readMem temperature failed\n";
		return 1;
	}

	std::cout << "OK raw=0x" << std::hex << (raw & 0xFFFF) << std::dec
		  << " temp_c=" << tempC << "\n";
	return 0;
}
