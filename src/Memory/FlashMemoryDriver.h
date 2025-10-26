/*
 * BaseMemory.h
 *
 *  Created on: Oct 25, 2025
 *      Author: alesh
 */

#ifndef FLASHMEMORYDRIVER_H_
#define FLASHMEMORYDRIVER_H_

#define FLASH_BLOCK_SIZE FLASH_PAGE_SIZE
#define FLASH_BLOCK_COUNT 64

#include "Memory/BaseMemoryDriver.h"
#include "Memory/Storage.h"
#include "Adapter/System.h"

namespace IntroSatLib::memory {

class FlashMemoryDriver: public MemoryDriver {
	MemoryCellSize cellSize = MEM_CELL_2B;
	const size_t PageSize = FLASH_PAGE_SIZE;
	const size_t PageCount = FLASH_BLOCK_COUNT;
	const size_t flashStart = FLASH_BASE;

	static size_t memoryPagesUsed;

	struct MemoryPage {
		size_t start;
		size_t end;
		uint64_t writeCycles;
		uint64_t maxWriteCycles = UINT64_MAX; //TODO use something real
	};

	MemoryPage* memoryPages;
	size_t memoryPagesCount;


	bool IsValidFlashAddress(size_t virtualAddress) {
		return (virtualAddress < flashStart + (PageSize * PageCount)) && (virtualAddress > flashStart);
	}

	bool IsAvailableFlashAddress(size_t virtualAddress) {
		return (virtualAddress < memoryPagesCount * PageSize);
	}

	MemoryOperationStatus VirtualAddressToRealAddress(size_t virtualAddress, size_t* realAddress) {
		if (!IsAvailableFlashAddress(virtualAddress)) return MEM_OUT_OF_RANGE;
		size_t pageIndex = virtualAddress / PageSize;
		size_t addrOnPage = (virtualAddress % PageSize);
		*realAddress = memoryPages[pageIndex].start + addrOnPage;
		return MEM_OK;
	}

public:
	FlashMemoryDriver() {

	}

	MemoryInitStatus Init(size_t size) override {
		memoryPagesCount = (size + 1) / PageSize;
		if ((memoryPagesCount + memoryPagesUsed) > PageCount) return MEM_INIT_NOT_ENOUGH_SPACE;

		memoryPages = new MemoryPage[memoryPagesCount];

		if (!IsValidFlashAddress(flashStart + (PageCount - (memoryPagesUsed + memoryPagesCount)) * PageSize)) return MEM_INIT_NOT_ENOUGH_SPACE;

		for (uint8_t i = 0; i < memoryPagesCount; i++) {
			memoryPages[i].start = flashStart + (PageCount - (memoryPagesUsed + i) - 1) * PageSize;
			memoryPages[i].end = flashStart + (PageCount - (memoryPagesUsed + i)) * PageSize - 1;
		}

//		if (!IsPageEmpty(memoryPages[memoryPagesCount - 1])) return MEM_INIT_NOT_ENOUGH_SPACE;
		memoryPagesUsed += memoryPagesCount;
		return MEM_INIT_OK;
	}

	MemoryOperationStatus read(size_t virtualAddress, uint32_t* value) override {
		size_t realAddress;
		MemoryOperationStatus status = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (status != MEM_OK) return status;

		*value = (*(__IO uint16_t*) realAddress);

		return MEM_OK;
	}

	MemoryOperationStatus write(size_t virtualAddress, uint32_t value) override {
		size_t realAddress;
		MemoryOperationStatus memStatus = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (memStatus != MEM_OK) return memStatus;

		if (!IsEmpty(virtualAddress)) return MEM_TAKEN;

		if (HAL_FLASH_Unlock() != HAL_OK) return MEM_LOCKED;


		HAL_StatusTypeDef status =  HAL_FLASH_Program(	FLASH_TYPEPROGRAM_HALFWORD,
														realAddress,
														value);

		HAL_FLASH_Lock();

		return (status == HAL_OK) ? MEM_OK : MEM_ERROR;

	};

	bool IsEmpty(size_t virtualAddress) {
		uint32_t value;
		if (read(virtualAddress, &value) != MEM_OK) return false;

		if (value != 0xFFFF) return false;

		return true;
	}

	bool IsPageEmpty(MemoryPage page) {
		if (!IsValidFlashAddress(page.start)) return false;
		size_t address = page.start;
		while (address < page.end) {
			if (*((__IO uint16_t*) address) != 0xFFFF) return false;
		}
		return true;
	}

	MemoryOperationStatus ErasePage(MemoryPage page) {
		FLASH_EraseInitTypeDef eraseInitTypeDef;
		uint32_t faultyPage = 0;
		HAL_StatusTypeDef status;

		eraseInitTypeDef.PageAddress = page.start;
		eraseInitTypeDef.NbPages = 1;
		eraseInitTypeDef.TypeErase = FLASH_TYPEERASE_PAGES;

		status = HAL_FLASH_Unlock();
		if (status != HAL_OK) return MEM_LOCKED;

		status = HAL_FLASHEx_Erase(&eraseInitTypeDef, &faultyPage);
		if (status != HAL_OK) return MEM_ERROR;

		return HAL_FLASH_Lock() == HAL_OK ? MEM_OK : MEM_ERROR;
	}

	MemoryOperationStatus EraseAll() override {
		MemoryOperationStatus status;
		for (size_t i = 0; i < memoryPagesCount; i++) {
			status = ErasePage(memoryPages[i]);
			if (status != MEM_OK) return status;
		}
		return MEM_OK;
	}


};
size_t FlashMemoryDriver::memoryPagesUsed = 0;

//FlashMemoryDriver<>* getFlashDriver() {
//	static FlashMemoryDriver<>* flashDriver = new FlashMemoryDriver<>();
//	return flashDriver;
//}
//template <typename DataType = uint8_t>
//using FlashStorage = Storage<FlashMemoryDriver<>, DataType>;


}


#endif
