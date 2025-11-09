#include "Adapter/SPI.h"

#ifdef ISL_SPI_ENABLED

#include "W25Q.h"

#include "stdio.h"
#include "string.h"

namespace IntroSatLib {

void W25Q::Reset(void) {
	Select();
	tx_buf[0] = W25_CMD_ENABLE_RESET;
	tx_buf[1] = W25_CMD_RESET;
	TransmitSPI(tx_buf, 2);
	Deselect();
}

ISL_StatusTypeDef W25Q::ReadData(uint32_t addr, uint8_t *data, uint32_t sz) {
	Select();
	tx_buf[0] = W25_CMD_READ;
	tx_buf[1] = (addr >> 16) & 0xFF;
	tx_buf[2] = (addr >> 8) & 0xFF;
	tx_buf[3] = addr & 0xFF;
	ISL_StatusTypeDef status = TransmitSPI(tx_buf, 4);
	if (status != ISL_OK) {
		return status;
	}
	status = ReceiveSPI(data, sz);
	Deselect();
	return status;
}

ISL_StatusTypeDef W25Q::Init(void) {
	system::Delay(100); //? neccessary?
	Reset();
	system::Delay(100);
	uint32_t id = ReadID();
	if (((id >> 16) & 0xff) != 0xEF)
		return ISL_ERROR; // Check if MSB of id matches with the correct one from specification
	id &= 0x0000ffff;
	highCap = 0;
	switch (id) {
	case 0x401A:
		blockCount = 1024;
		variant = W25Q512;
		highCap = 1;
		break;
	case 0x4019:
		blockCount = 512;
		variant = W25Q256;
		highCap = 1;
		break;
	case 0x4018:
		blockCount = 256;
		variant = W25Q128;
		break;
	case 0x4017:
		blockCount = 128;
		variant = W25Q64;
		break;
	case 0x4016:
		blockCount = 64;
		variant = W25Q32;
		break;
	case 0x4015:
		blockCount = 32;
		variant = W25Q16;
		break;
	case 0x4014:
		blockCount = 16;
		variant = W25Q80;
		break;
	case 0x4013:
		blockCount = 8;
		variant = W25Q40;
		break;
	case 0x4012:
		blockCount = 4;
		variant = W25Q20;
		break;
	case 0x4011:
		blockCount = 2;
		variant = W25Q10;
		break;
	default:
		variant = UNKNOWN;
		return ISL_ERROR;
		break;
	}
	pageSize = 256;
	sectorSize = 0x1000;
	sectorCount = blockCount * 16;
	pageCount = (sectorCount * sectorSize) / pageSize;
	blockSize = sectorSize * 16;
	numKB = (sectorCount / 1024 * sectorSize);
	return ISL_OK;
}

uint32_t W25Q::ReadID(void) {
	uint8_t dt[4];
	tx_buf[0] = W25_CMD_GET_JEDEC_ID;
	Select();
	TransmitSPI(tx_buf, 1);
	ReceiveSPI(dt, 3);
	Deselect();
	return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}

W25Q::W25Q(interfaces::SPI _spi): SPIDevice(_spi) { }

#ifdef ISL_UART_ENABLED
void W25Q::DumpChipInfo(interfaces::UART& uart) {
	char str1[40];
	uint32_t id = ReadID();
    
	uart.transmit((uint8_t*) "W25Q Chip Info:\n\r",
			strlen("W25Q Chip Info:\n\r"), 300);
	sprintf(str1, "\tChip ID: 0x%X%X\n\r", (uint16_t)((id >> 16) & 0xffff), (uint16_t)(id & 0xffff));
	uart.transmit((uint8_t*) str1, strlen(str1), 300);
	sprintf(str1, "\tVariant: W25Q%ld\n\r", blockCount / 2);
	uart.transmit((uint8_t*) str1, strlen(str1), 300);
	sprintf(str1, "\tPage Size: %u Bytes\r\n", pageSize);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tPage Count: %ld\r\n", pageCount);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tSector Size: %ld Bytes\r\n", sectorSize);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tSector Count: %ld\r\n", sectorCount);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tBlock Size: %ld Bytes\r\n", blockSize);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tBlock Count: %ld\r\n", blockCount);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tCapacity: %ld KB\r\n", numKB);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
	sprintf(str1, "\tHigh Capacity: %u\r\n", highCap);
	uart.transmit((uint8_t*) str1, strlen(str1), 0x1000);
}
#endif /* ISL_UART_ENABLED */

ISL_StatusTypeDef W25Q::ReadPage(uint8_t *data, uint32_t pageAddr, uint32_t offset, uint32_t sz) {
	if (sz > pageSize)
		sz = pageSize;
	if ((offset + sz) > pageSize)
		sz = pageSize - offset;
	pageAddr = pageAddr * pageSize + offset;
	tx_buf[0] = W25_CMD_FAST_READ;
    Select();
    if(highCap)
    {
        tx_buf[1] = (pageAddr >> 24) & 0xFF;
        tx_buf[2] = (pageAddr >> 16) & 0xFF;
        tx_buf[3] = (pageAddr >> 8) & 0xFF;
        tx_buf[4] = pageAddr & 0xFF;
        tx_buf[5] = 0;
        TransmitSPI(tx_buf, 6);
    }
    else
    {
        tx_buf[1] = (pageAddr >> 16) & 0xFF;
        tx_buf[2] = (pageAddr >> 8) & 0xFF;
        tx_buf[3] = pageAddr & 0xFF;
        tx_buf[4] = 0;
        TransmitSPI(tx_buf, 5);
    }
    ISL_StatusTypeDef status = ReceiveSPI(data, sz);
    Deselect();
    return status;
}

ISL_StatusTypeDef W25Q::WritePage(uint8_t *data, uint32_t pageAddr)
{
    waitWriteEnd();
    Select();
    tx_buf[0] = W25_CMD_PAGE_PROGRAMM;
    uint32_t addr = pageAddr * pageSize;
    if(highCap)
    {
        tx_buf[1] = (addr >> 24) & 0xFF;
        tx_buf[2] = (addr >> 16) & 0xFF;
        tx_buf[3] = (addr >> 8) & 0xFF;
        tx_buf[4] = addr & 0xFF;
        TransmitSPI(tx_buf, 5);
    }
    else
    {
        tx_buf[1] = (addr >> 16) & 0xFF;
        tx_buf[2] = (addr >> 8) & 0xFF;
        tx_buf[3] = addr & 0xFF;
        TransmitSPI(tx_buf, 4);
    }  
    ISL_StatusTypeDef status = TransmitSPI(data, pageSize);
    waitWriteEnd();
    return status;
}

void W25Q::writeEnable(void) {
	Select();
	rx_buf[0] = W25_CMD_WRITE_ENABLE;
	TransmitSPI(rx_buf, 1);
	Deselect();
	system::Delay(1); // TODO: Maybe unnecessary?
}

void W25Q::writeDisable(void) {
	Select();
	rx_buf[0] = W25_CMD_WRITE_DISABLE;
	TransmitSPI(rx_buf, 1);
	Deselect();
	system::Delay(1); // TODO: Maybe unnecessary?
}

ISL_StatusTypeDef W25Q::waitWriteEnd(void) {
	system::Delay(1);
	Select();
	tx_buf[0] = W25_CMD_READ_STATUS_1;
	ISL_StatusTypeDef status = TransmitSPI(tx_buf, 1);
	do {
		status = ReceiveSPI(rx_buf, 1);
		SR1 = rx_buf[0];
		system::Delay(1);
	} while ((SR1 & 0x01) == 0x01);
	Deselect();
	return status;
}

void W25Q::EraseSector(uint32_t addr) {
	waitWriteEnd();
	setBlockProtect(0x00);
	addr = addr * sectorSize;
	writeEnable();
	Select();
	tx_buf[0] = W25_CMD_SECTOR_ERASE;
	if(highCap) {
		tx_buf[1] = (addr >> 24) & 0xFF;
		tx_buf[2] = (addr >> 16) & 0xFF;
		tx_buf[3] = (addr >> 8) & 0xFF;
		tx_buf[4] = addr & 0xFF;
		TransmitSPI(tx_buf, 5);
	} else {
		tx_buf[1] = (addr >> 16) & 0xFF;
		tx_buf[2] = (addr >> 8) & 0xFF;
		tx_buf[3] = addr & 0xFF;
		TransmitSPI(tx_buf, 4);
	}
	Deselect();
	waitWriteEnd();
	system::Delay(1); // TODO: Maybe unnecessary?
	writeDisable();
	setBlockProtect(0x0F);
}

void W25Q::EraseBlock(uint32_t addr) {
	waitWriteEnd();
	addr = addr * blockSize;
	writeEnable();
	Select();
	tx_buf[0] = W25_CMD_BLOCK_ERASE;
	if(highCap)
	{
		tx_buf[1] = (addr >> 24) & 0xFF;
		tx_buf[2] = (addr >> 16) & 0xFF;
		tx_buf[3] = (addr >> 8) & 0xFF;
		tx_buf[4] = addr & 0xFF;
		TransmitSPI(tx_buf, 5);
	}
	else
	{
		tx_buf[1] = (addr >> 16) & 0xFF;
		tx_buf[2] = (addr >> 8) & 0xFF;
		tx_buf[3] = addr & 0xFF;
		TransmitSPI(tx_buf, 4);
	}
	Deselect();
	waitWriteEnd();
	system::Delay(1);

}

void W25Q::EraseChip(void) {
	waitWriteEnd();
	writeEnable();
	Select();
	tx_buf[0] = W25_CMD_CHIP_ERASE;
	TransmitSPI(tx_buf, 1);
	Deselect();
	waitWriteEnd();
	system::Delay(10);
}

void W25Q::setBlockProtect(uint8_t val) {
	tx_buf[0] = 0x50;
	Select();
	TransmitSPI(tx_buf, 1);
	Deselect();
	tx_buf[0] = W25_CMD_WRITE_STATUS_1;
	tx_buf[1] = ((val & 0x0F) << 2);
	Select();
	TransmitSPI(tx_buf, 2);
	Deselect();
}

} /* namespace IntroSatLib */
#endif /* ISL_SPI_ENABLED */
