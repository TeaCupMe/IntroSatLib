#include "TcpIspServer.h"
#include "TcpSocketIO.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace IntroSatLib {
namespace headless {

TcpIspServer::TcpIspServer(uint16_t port) : port_(port) {}

TcpIspServer::~TcpIspServer() {
	close();
}

void TcpIspServer::close() {
	if (listen_fd_ >= 0) {
		::close(listen_fd_);
		listen_fd_ = -1;
	}
}

bool TcpIspServer::listen() {
	close();
	listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd_ < 0) {
		return false;
	}
	int yes = 1;
	::setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(port_);
	if (::bind(listen_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
		close();
		return false;
	}
	if (::listen(listen_fd_, 1) != 0) {
		close();
		return false;
	}
	return true;
}

bool TcpIspServer::serveOne(const Handler& handler) {
	if (listen_fd_ < 0 || !handler) {
		return false;
	}

	sockaddr_in client {};
	socklen_t clientLen = sizeof(client);
	const int clientFd =
		::accept(listen_fd_, reinterpret_cast<sockaddr*>(&client), &clientLen);
	if (clientFd < 0) {
		return false;
	}

	for (;;) {
		uint8_t lenBuf[4];
		if (!tcp::readAll(clientFd, lenBuf, 4)) {
			break;
		}
		const uint32_t bodyLen = isp::readU32LE(lenBuf);
		if (bodyLen == 0 || bodyLen > isp::kMaxFrameBody) {
			break;
		}
		uint8_t body[isp::kMaxFrameBody];
		if (!tcp::readAll(clientFd, body, bodyLen)) {
			break;
		}

		isp::Request req;
		if (!isp::decodeRequestBody(body, bodyLen, req)) {
			break;
		}

		isp::Response resp;
		if (req.op == isp::Op::Ping) {
			resp.status = ISL_OK;
			resp.n = 0;
		} else {
			resp = handler(req);
			if (resp.n > isp::kMaxPayload) {
				resp.status = ISL_ERROR;
				resp.n = 0;
			}
		}

		uint8_t outBody[isp::kMaxFrameBody];
		uint32_t outLen = 0;
		if (!isp::encodeResponseBody(resp, req.op, outBody, sizeof(outBody), outLen)) {
			break;
		}
		uint8_t outLenBuf[4];
		isp::writeU32LE(outLenBuf, outLen);
		if (!tcp::writeAll(clientFd, outLenBuf, 4) ||
		    !tcp::writeAll(clientFd, outBody, outLen)) {
			break;
		}
	}

	::close(clientFd);
	return true;
}

} /* namespace headless */
} /* namespace IntroSatLib */
