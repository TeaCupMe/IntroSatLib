#ifndef HEADLESS_TCP_TCPISPSERVER_H_
#define HEADLESS_TCP_TCPISPSERVER_H_

#include "IspLite.h"
#include <cstdint>
#include <functional>

namespace IntroSatLib {
namespace headless {

/** Minimal loopback TCP server: one client, ISP-lite request/response. */
class TcpIspServer {
public:
	using Handler = std::function<isp::Response(const isp::Request&)>;

	explicit TcpIspServer(uint16_t port);
	~TcpIspServer();

	TcpIspServer(const TcpIspServer&) = delete;
	TcpIspServer& operator=(const TcpIspServer&) = delete;

	bool listen();
	bool serveOne(const Handler& handler);
	void close();

private:
	uint16_t port_ = 0;
	int listen_fd_ = -1;
};

} /* namespace headless */
} /* namespace IntroSatLib */

#endif /* HEADLESS_TCP_TCPISPSERVER_H_ */
