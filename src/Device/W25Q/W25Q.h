#ifndef DEVICE_W25Q_H_
#define DEVICE_W25Q_H_

#include "stdint.h"
#include "IntroSatLib_def.h"
#include "Adapter/SPI.h"
#include "Adapter/UART.h"
#include "Adapter/GPIO.h"
#include "Adapter/System.h"
#include "Device/SPIDevice.h"

namespace IntroSatLib {

class W25Q: public SPIDevice {

enum W25QXX {
	W25Q512,
	W25Q256,
	W25Q128,
	W25Q64,
	W25Q32,
	W25Q16,
	W25Q80,
	W25Q40,
	W25Q20,
	W25Q10,
	UNKNOWN
};

enum W25Q_Command {
    W25_CMD_READ             = 0x03,
    W25_CMD_FAST_READ        = 0x0B,
    W25_CMD_ENABLE_RESET     = 0x66,
    W25_CMD_RESET            = 0x99,
    W25_CMD_GET_JEDEC_ID     = 0x9f,
    W25_CMD_READ_STATUS_1    = 0x05,
    W25_CMD_READ_STATUS_2    = 0x35,
    W25_CMD_READ_STATUS_3    = 0x15,
    W25_CMD_WRITE_STATUS_1   = 0x01,
    W25_CMD_WRITE_STATUS_2   = 0x31,
    W25_CMD_WRITE_STATUS_3   = 0x11,
    W25_CMD_PAGE_PROGRAMM    = 0x02,
    W25_CMD_WRITE_DISABLE    = 0x04,
    W25_CMD_WRITE_ENABLE     = 0x06,
    W25_CMD_SECTOR_ERASE     = 0x20,
    W25_CMD_BLOCK_ERASE      = 0xD8,
    W25_CMD_CHIP_ERASE       = 0xC7,
};

public:
	
	W25QXX variant;
	uint16_t  pageSize;
	uint32_t  pageCount;
	uint32_t  sectorSize;
	uint32_t  sectorCount;
	uint32_t  blockSize;
	uint32_t  blockCount;
	uint32_t  numKB;
	uint8_t   SR1;
	uint8_t   SR2;
	uint8_t   SR3;
	uint8_t   highCap;

	W25Q(interfaces::SPI _spi);
    void SetCs(interfaces::GPIO_HANDLE_TYPE _csPort, uint16_t _csPin = 0);

	void Reset(void);
	ISL_StatusTypeDef ReadData(uint32_t addr, uint8_t* data, uint32_t sz);
	ISL_StatusTypeDef Init(void);
	uint32_t ReadID(void);
	
    ISL_StatusTypeDef ReadPage(uint8_t* data, uint32_t pageAddr, uint32_t offset, uint32_t sz);
	ISL_StatusTypeDef WritePage(uint8_t* data, uint32_t pageAddr);
    
    void EraseSector(uint32_t addr);
	void EraseBlock(uint32_t addr);
	void EraseChip(void);

#if defined(ISL_UART_ENABLED) and defined(DEBUG)
    void DumpChipInfo(interfaces::UART& _uart);
#endif /* ISL_UART_ENABLED */

private:
	uint8_t rx_buf[1025];
	uint8_t tx_buf[10];


	void writeEnable(void);
	void writeDisable(void);
	ISL_StatusTypeDef waitWriteEnd(void);
	void setBlockProtect(uint8_t val);



};
}; /* namespace IntroSatLib */
#endif /* DEVICE_W25Q_H_ */
