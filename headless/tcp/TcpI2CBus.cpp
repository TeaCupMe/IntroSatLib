#include "TcpI2CBus.h"
#include "TcpSocketIO.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace IntroSatLib {
namespace headless {

TcpI2CBus::TcpI2CBus(std::string host, uint16_t port)
	: host_(std::move(host)), port_(port) {}

TcpI2CBus::~TcpI2CBus() {
	disconnect();
}

void TcpI2CBus::disconnect() {
	if (fd_ >= 0) {
		::close(fd_);
		fd_ = -1;
	}
}

bool TcpI2CBus::connect() {
	disconnect();
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0) {
		return false;
	}
	sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	if (::inet_pton(AF_INET, host_.c_str(), &addr.sin_addr) != 1) {
		disconnect();
		return false;
	}
	if (::connect(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
		disconnect();
		return false;
	}
	return true;
}

ISL_StatusTypeDef TcpI2CBus::transact(isp::Request& req, isp::Response& resp) {
	if (fd_ < 0) {
		return ISL_ERROR;
	}

	uint8_t body[isp::kMaxFrameBody];
	uint32_t bodyLen = 0;
	if (!isp::encodeRequestBody(req, body, sizeof(body), bodyLen)) {
		return ISL_ERROR;
	}

	uint8_t lenBuf[4];
	isp::writeU32LE(lenBuf, bodyLen);
	if (!tcp::writeAll(fd_, lenBuf, 4) || !tcp::writeAll(fd_, body, bodyLen)) {
		disconnect();
		return ISL_ERROR;
	}

	uint8_t respLenBuf[4];
	if (!tcp::readAll(fd_, respLenBuf, 4)) {
		disconnect();
		return ISL_TIMEOUT;
	}
	const uint32_t respBodyLen = isp::readU32LE(respLenBuf);
	if (respBodyLen == 0 || respBodyLen > isp::kMaxFrameBody) {
		disconnect();
		return ISL_ERROR;
	}
	uint8_t respBody[isp::kMaxFrameBody];
	if (!tcp::readAll(fd_, respBody, respBodyLen)) {
		disconnect();
		return ISL_TIMEOUT;
	}
	if (!isp::decodeResponseBody(respBody, respBodyLen, resp)) {
		return ISL_ERROR;
	}
	return resp.status;
}

ISL_StatusTypeDef TcpI2CBus::isReady(uint8_t deviceAddress, uint8_t waitIsReady) {
	UNUSED(waitIsReady);
	isp::Request req;
	req.op = isp::Op::IsReady;
	req.addr = deviceAddress;
	isp::Response resp;
	return transact(req, resp);
}

ISL_StatusTypeDef TcpI2CBus::read(uint8_t deviceAddress, uint8_t* data,
				  uint8_t nBytes) {
	if (data == nullptr || nBytes == 0 || nBytes > isp::kMaxPayload) {
		return ISL_ERROR;
	}
	isp::Request req;
	req.op = isp::Op::Read;
	req.addr = deviceAddress;
	req.n = nBytes;
	isp::Response resp;
	const ISL_StatusTypeDef st = transact(req, resp);
	if (st != ISL_OK || resp.n != nBytes) {
		return (st != ISL_OK) ? st : ISL_ERROR;
	}
	std::memcpy(data, resp.data, nBytes);
	return ISL_OK;
}

ISL_StatusTypeDef TcpI2CBus::readMem(uint8_t deviceAddress, uint8_t reg,
				     uint8_t* data, uint8_t nBytes) {
	if (data == nullptr || nBytes == 0 || nBytes > isp::kMaxPayload) {
		return ISL_ERROR;
	}
	isp::Request req;
	req.op = isp::Op::ReadMem;
	req.addr = deviceAddress;
	req.reg = reg;
	req.n = nBytes;
	isp::Response resp;
	const ISL_StatusTypeDef st = transact(req, resp);
	if (st != ISL_OK || resp.n != nBytes) {
		return (st != ISL_OK) ? st : ISL_ERROR;
	}
	std::memcpy(data, resp.data, nBytes);
	return ISL_OK;
}

ISL_StatusTypeDef TcpI2CBus::write(uint8_t deviceAddress, uint8_t* data,
				   uint8_t nBytes) {
	if (data == nullptr || nBytes == 0 || nBytes > isp::kMaxPayload) {
		return ISL_ERROR;
	}
	isp::Request req;
	req.op = isp::Op::Write;
	req.addr = deviceAddress;
	req.n = nBytes;
	std::memcpy(req.data, data, nBytes);
	isp::Response resp;
	return transact(req, resp);
}

ISL_StatusTypeDef TcpI2CBus::writeMem(uint8_t deviceAddress, uint8_t reg,
				      uint8_t* data, uint8_t nBytes) {
	if (data == nullptr || nBytes == 0 || nBytes > isp::kMaxPayload) {
		return ISL_ERROR;
	}
	isp::Request req;
	req.op = isp::Op::WriteMem;
	req.addr = deviceAddress;
	req.reg = reg;
	req.n = nBytes;
	std::memcpy(req.data, data, nBytes);
	isp::Response resp;
	return transact(req, resp);
}

} /* namespace headless */
} /* namespace IntroSatLib */
