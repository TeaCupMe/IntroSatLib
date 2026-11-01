#ifndef HEADLESS_TCP_TCPI2CBUS_H_
#define HEADLESS_TCP_TCPI2CBUS_H_

#include "I2CBus.h"
#include "IspLite.h"
#include <cstdint>
#include <string>

namespace IntroSatLib {
namespace headless {

/**
 * Host I2CBus over localhost TCP + ISP-lite framing.
 * Headless-only; not part of production Adapter tree.
 */
class TcpI2CBus final : public interfaces::I2CBus {
public:
	TcpI2CBus(std::string host, uint16_t port);
	~TcpI2CBus() override;

	TcpI2CBus(const TcpI2CBus&) = delete;
	TcpI2CBus& operator=(const TcpI2CBus&) = delete;

	bool connect();
	void disconnect();
	bool isConnected() const { return fd_ >= 0; }

	ISL_StatusTypeDef isReady(uint8_t deviceAddress,
				  uint8_t waitIsReady = 0) override;
	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data,
			       uint8_t nBytes) override;
	ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg,
				  uint8_t* data, uint8_t nBytes) override;
	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data,
				uint8_t nBytes) override;
	ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg,
				   uint8_t* data, uint8_t nBytes) override;

private:
	ISL_StatusTypeDef transact(isp::Request& req, isp::Response& resp);

	std::string host_;
	uint16_t port_ = 0;
	int fd_ = -1;
};

} /* namespace headless */
} /* namespace IntroSatLib */

#endif /* HEADLESS_TCP_TCPI2CBUS_H_ */
