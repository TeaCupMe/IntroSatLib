#ifndef HEADLESS_TCP_TCPSOCKETIO_H_
#define HEADLESS_TCP_TCPSOCKETIO_H_

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>

namespace IntroSatLib {
namespace headless {
namespace tcp {

inline bool writeAll(int fd, const uint8_t* data, std::size_t len) {
	std::size_t off = 0;
	while (off < len) {
		const ssize_t n = ::send(fd, data + off, len - off, 0);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return false;
		}
		if (n == 0) {
			return false;
		}
		off += static_cast<std::size_t>(n);
	}
	return true;
}

inline bool readAll(int fd, uint8_t* data, std::size_t len) {
	std::size_t off = 0;
	while (off < len) {
		const ssize_t n = ::recv(fd, data + off, len - off, 0);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return false;
		}
		if (n == 0) {
			return false;
		}
		off += static_cast<std::size_t>(n);
	}
	return true;
}

} /* namespace tcp */
} /* namespace headless */
} /* namespace IntroSatLib */

#endif /* HEADLESS_TCP_TCPSOCKETIO_H_ */
