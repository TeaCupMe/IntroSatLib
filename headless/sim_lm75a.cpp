#include "IspLite.h"
#include "TcpIspServer.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr uint8_t kDefaultAddr = 0x48;
constexpr uint8_t kRegTemp = 0x00;
constexpr uint8_t kRegConfig = 0x01;
constexpr uint16_t kDefaultPort = 17375;

/** LM75A raw temp: degrees C * 256 (MSB = integer °C). Default 25.0 °C. */
constexpr uint16_t kDefaultTempRaw = 0x1900;

struct Lm75aSim {
	uint8_t addr = kDefaultAddr;
	std::unordered_map<uint8_t, std::vector<uint8_t>> regs;

	Lm75aSim() {
		regs[kRegTemp] = {0x19, 0x00}; // 25.0 °C
		regs[kRegConfig] = {0x00};
		(void)kDefaultTempRaw;
	}

	IntroSatLib::headless::isp::Response handle(
		const IntroSatLib::headless::isp::Request& req) {
		using IntroSatLib::headless::isp::Op;
		using IntroSatLib::headless::isp::Response;
		Response resp;
		resp.status = ISL_ERROR;
		resp.n = 0;

		if (req.addr != addr) {
			return resp;
		}

		switch (req.op) {
		case Op::IsReady:
			resp.status = ISL_OK;
			return resp;
		case Op::ReadMem: {
			auto it = regs.find(req.reg);
			if (it == regs.end() || req.n == 0 || req.n > it->second.size()) {
				return resp;
			}
			resp.status = ISL_OK;
			resp.n = req.n;
			std::memcpy(resp.data, it->second.data(), req.n);
			return resp;
		}
		case Op::WriteMem: {
			if (req.n == 0) {
				return resp;
			}
			regs[req.reg].assign(req.data, req.data + req.n);
			resp.status = ISL_OK;
			return resp;
		}
		case Op::Read: {
			// Current pointer = last accessed; MVP: always temp.
			auto& temp = regs[kRegTemp];
			if (req.n == 0 || req.n > temp.size()) {
				return resp;
			}
			resp.status = ISL_OK;
			resp.n = req.n;
			std::memcpy(resp.data, temp.data(), req.n);
			return resp;
		}
		case Op::Write: {
			if (req.n == 0) {
				return resp;
			}
			// Treat as pointer + optional data write to that reg.
			const uint8_t reg = req.data[0];
			if (req.n == 1) {
				regs[reg]; // ensure exists
				resp.status = ISL_OK;
				return resp;
			}
			regs[reg].assign(req.data + 1, req.data + req.n);
			resp.status = ISL_OK;
			return resp;
		}
		default:
			return resp;
		}
	}
};

void usage(const char* argv0) {
	std::cerr << "Usage: " << argv0 << " [--port PORT] [--addr HEX]\n"
		  << "  Default port " << kDefaultPort << ", addr 0x48\n";
}

} // namespace

int main(int argc, char** argv) {
	uint16_t port = kDefaultPort;
	uint8_t addr = kDefaultAddr;

	for (int i = 1; i < argc; ++i) {
		const std::string arg = argv[i];
		if (arg == "--port" && i + 1 < argc) {
			port = static_cast<uint16_t>(std::stoi(argv[++i]));
		} else if (arg == "--addr" && i + 1 < argc) {
			addr = static_cast<uint8_t>(std::stoul(argv[++i], nullptr, 0));
		} else if (arg == "--help" || arg == "-h") {
			usage(argv[0]);
			return 0;
		} else {
			usage(argv[0]);
			return 1;
		}
	}

	Lm75aSim sim;
	sim.addr = addr;

	IntroSatLib::headless::TcpIspServer server(port);
	if (!server.listen()) {
		std::cerr << "sim_lm75a: failed to listen on 127.0.0.1:" << port << "\n";
		return 1;
	}
	std::cout << "introsatlib_sim_lm75a listening on 127.0.0.1:" << port
		  << " addr=0x" << std::hex << static_cast<int>(addr) << std::dec
		  << " (temp raw=0x1900 ≈ 25.0 C)\n";

	if (!server.serveOne([&](const auto& req) { return sim.handle(req); })) {
		std::cerr << "sim_lm75a: accept/serve failed\n";
		return 1;
	}
	std::cout << "sim_lm75a: client disconnected\n";
	return 0;
}
