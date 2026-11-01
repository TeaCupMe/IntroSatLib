#ifndef HEADLESS_ISP_ISPLITE_H_
#define HEADLESS_ISP_ISPLITE_H_

#include "IntroSatLib_def.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace IntroSatLib {
namespace headless {
namespace isp {

/** ISP-lite framing (see docs/headless-ipc-backend.md §5). Little-endian. */
inline constexpr uint8_t kVersion = 1;
inline constexpr uint8_t kMaxPayload = 32;
inline constexpr uint32_t kMaxFrameBody = 4u + 3u + kMaxPayload; // ver+op+flags + worst payload

enum class Op : uint8_t {
	IsReady = 1,
	Read = 2,
	ReadMem = 3,
	Write = 4,
	WriteMem = 5,
	Ping = 6,
};

struct Request {
	Op op = Op::Ping;
	uint16_t flags = 0;
	uint8_t addr = 0;
	uint8_t reg = 0;
	uint8_t n = 0;
	uint8_t data[kMaxPayload] = {};
};

struct Response {
	ISL_StatusTypeDef status = ISL_ERROR;
	uint8_t n = 0;
	uint8_t data[kMaxPayload] = {};
};

inline void writeU16LE(uint8_t* p, uint16_t v) {
	p[0] = static_cast<uint8_t>(v & 0xFFu);
	p[1] = static_cast<uint8_t>((v >> 8) & 0xFFu);
}

inline uint16_t readU16LE(const uint8_t* p) {
	return static_cast<uint16_t>(p[0]) |
	       (static_cast<uint16_t>(p[1]) << 8);
}

inline void writeU32LE(uint8_t* p, uint32_t v) {
	p[0] = static_cast<uint8_t>(v & 0xFFu);
	p[1] = static_cast<uint8_t>((v >> 8) & 0xFFu);
	p[2] = static_cast<uint8_t>((v >> 16) & 0xFFu);
	p[3] = static_cast<uint8_t>((v >> 24) & 0xFFu);
}

inline uint32_t readU32LE(const uint8_t* p) {
	return static_cast<uint32_t>(p[0]) |
	       (static_cast<uint32_t>(p[1]) << 8) |
	       (static_cast<uint32_t>(p[2]) << 16) |
	       (static_cast<uint32_t>(p[3]) << 24);
}

inline bool encodeRequestBody(const Request& req, uint8_t* body, std::size_t cap,
			      uint32_t& bodyLen) {
	if (req.n > kMaxPayload) {
		return false;
	}
	// body: u8 ver, u8 op, u16 flags, payload…
	std::size_t need = 4;
	switch (req.op) {
	case Op::Ping:
		break;
	case Op::IsReady:
		need += 1;
		break;
	case Op::Read:
		need += 2;
		break;
	case Op::ReadMem:
		need += 3;
		break;
	case Op::Write:
		need += 2u + req.n;
		break;
	case Op::WriteMem:
		need += 3u + req.n;
		break;
	default:
		return false;
	}
	if (cap < need) {
		return false;
	}
	body[0] = kVersion;
	body[1] = static_cast<uint8_t>(req.op);
	writeU16LE(body + 2, req.flags);
	std::size_t off = 4;
	auto put = [&](uint8_t b) { body[off++] = b; };
	switch (req.op) {
	case Op::Ping:
		break;
	case Op::IsReady:
		put(req.addr);
		break;
	case Op::Read:
		put(req.addr);
		put(req.n);
		break;
	case Op::ReadMem:
		put(req.addr);
		put(req.reg);
		put(req.n);
		break;
	case Op::Write:
		put(req.addr);
		put(req.n);
		std::memcpy(body + off, req.data, req.n);
		off += req.n;
		break;
	case Op::WriteMem:
		put(req.addr);
		put(req.reg);
		put(req.n);
		std::memcpy(body + off, req.data, req.n);
		off += req.n;
		break;
	}
	bodyLen = static_cast<uint32_t>(off);
	return true;
}

inline bool decodeRequestBody(const uint8_t* body, uint32_t bodyLen, Request& req) {
	if (bodyLen < 4) {
		return false;
	}
	if (body[0] != kVersion) {
		return false;
	}
	req.op = static_cast<Op>(body[1]);
	req.flags = readU16LE(body + 2);
	req.addr = 0;
	req.reg = 0;
	req.n = 0;
	uint32_t off = 4;
	auto need = [&](uint32_t n) -> bool { return bodyLen >= off + n; };
	switch (req.op) {
	case Op::Ping:
		return bodyLen == 4;
	case Op::IsReady:
		if (!need(1)) {
			return false;
		}
		req.addr = body[off];
		return true;
	case Op::Read:
		if (!need(2)) {
			return false;
		}
		req.addr = body[off];
		req.n = body[off + 1];
		return req.n <= kMaxPayload;
	case Op::ReadMem:
		if (!need(3)) {
			return false;
		}
		req.addr = body[off];
		req.reg = body[off + 1];
		req.n = body[off + 2];
		return req.n <= kMaxPayload;
	case Op::Write:
		if (!need(2)) {
			return false;
		}
		req.addr = body[off];
		req.n = body[off + 1];
		if (req.n > kMaxPayload || !need(2u + req.n)) {
			return false;
		}
		std::memcpy(req.data, body + off + 2, req.n);
		return true;
	case Op::WriteMem:
		if (!need(3)) {
			return false;
		}
		req.addr = body[off];
		req.reg = body[off + 1];
		req.n = body[off + 2];
		if (req.n > kMaxPayload || !need(3u + req.n)) {
			return false;
		}
		std::memcpy(req.data, body + off + 3, req.n);
		return true;
	default:
		return false;
	}
}

inline bool encodeResponseBody(const Response& resp, Op /*reqOp*/, uint8_t* body,
			       std::size_t cap, uint32_t& bodyLen) {
	// Response body: u8 ver, u8 status, [u8 n, bytes…] when data present
	if (resp.n > kMaxPayload) {
		return false;
	}
	const std::size_t need = (resp.n > 0) ? (2u + 1u + resp.n) : 2u;
	if (cap < need) {
		return false;
	}
	body[0] = kVersion;
	body[1] = static_cast<uint8_t>(resp.status);
	if (resp.n > 0) {
		body[2] = resp.n;
		std::memcpy(body + 3, resp.data, resp.n);
		bodyLen = static_cast<uint32_t>(3u + resp.n);
	} else {
		bodyLen = 2;
	}
	return true;
}

inline bool decodeResponseBody(const uint8_t* body, uint32_t bodyLen, Response& resp) {
	if (bodyLen < 2 || body[0] != kVersion) {
		return false;
	}
	resp.status = static_cast<ISL_StatusTypeDef>(body[1]);
	resp.n = 0;
	if (bodyLen == 2) {
		return true;
	}
	if (bodyLen < 3) {
		return false;
	}
	resp.n = body[2];
	if (resp.n > kMaxPayload || bodyLen < 3u + resp.n) {
		return false;
	}
	std::memcpy(resp.data, body + 3, resp.n);
	return true;
}

} /* namespace isp */
} /* namespace headless */
} /* namespace IntroSatLib */

#endif /* HEADLESS_ISP_ISPLITE_H_ */
