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
// #include "Adapter/Flash.h"

namespace IntroSatLib::memory {
template <size_t FlashCellSize>
class FlashMemoryDriver: public MemoryDriver<FlashCellSize> {
	
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
	size_t memoryPagesCount = 0;


	 bool IsValidFlashAddress(size_t virtualAddress) {
	 	if (virtualAddress % FlashCellSize != 0) return false;
	 	return (virtualAddress < (PageSize * PageCount));
	 }

	// bool IsAvailableFlashAddress(size_t virtualAddress) {
	// 	return (virtualAddress < memoryPagesCount * PageSize);
	// }

	bool IsValidAddress(size_t virtualAddress) {
		if ((virtualAddress % FlashCellSize) != 0) return false;
		if (virtualAddress >= memoryPagesCount * PageSize) return false;

		return true;
	}

	MemoryOperationStatus VirtualAddressToRealAddress(size_t virtualAddress, size_t* realAddress) {
		if (!IsValidAddress(virtualAddress)) return MEM_OUT_OF_RANGE;
		size_t pageIndex = virtualAddress / PageSize;
		size_t addrOnPage = (virtualAddress % PageSize);
		*realAddress = memoryPages[pageIndex].start + addrOnPage;
		return MEM_OK;
	}

	// Written by GPT-4
	MemoryOperationStatus RealAddressToVirtualAddress(size_t realAddress, size_t* virtualAddress) {
		if (!IsValidAddress(realAddress)) return MEM_OUT_OF_RANGE;
		for (size_t i = 0; i < memoryPagesCount; i++) {
			if ((realAddress >= memoryPages[i].start) && (realAddress <= memoryPages[i].end)) {
				*virtualAddress = (i * PageSize) + (realAddress - memoryPages[i].start);
				return MEM_OK;
			}
		}
		return MEM_OUT_OF_RANGE;
	}

public:
	FlashMemoryDriver() {

	}

	MemoryInitStatus Init(size_t size) override {
		if (memoryPagesCount > 0) return MEM_INIT_REPEAT;
		memoryPagesCount = (size + PageSize - 1) / PageSize;
		if ((memoryPagesCount + memoryPagesUsed) > PageCount) return MEM_INIT_NOT_ENOUGH_SPACE;

		memoryPages = new MemoryPage[memoryPagesCount];

		if (!IsValidFlashAddress((PageCount - (memoryPagesUsed + memoryPagesCount)) * PageSize)) return MEM_INIT_NOT_ENOUGH_SPACE;

		for (uint8_t i = 0; i < memoryPagesCount; i++) {
			memoryPages[i].start = flashStart + (PageCount - (memoryPagesUsed + i) - 1) * PageSize;
			memoryPages[i].end = flashStart + (PageCount - (memoryPagesUsed + i)) * PageSize - 1;
		}

		memoryPagesUsed += memoryPagesCount;
		return MEM_INIT_OK;
	}

	MemoryOperationStatus read(size_t virtualAddress, MemoryDriver<FlashCellSize>::Cell* value) override {
		MemoryOperationStatus status;
		size_t realAddress;

		if (!(IsValidAddress(virtualAddress))) return MEM_OUT_OF_RANGE;

		status = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (status != MEM_OK) return status;

		for (size_t i = 0; i < sizeof(MemoryDriver<FlashCellSize>::Cell); i++) {
			*(((uint8_t*)value)+i) = *((__IO uint8_t*) realAddress);
		}

//		*value = *((__IO Cell*) realAddress);

		return MemoryOperationStatus::MEM_OK;
	}

	MemoryOperationStatus read(size_t virtualAddress, uint8_t* value, size_t len) override {
		MemoryOperationStatus status;
		size_t realAddress;

		if (!(IsValidAddress(virtualAddress))) return MEM_OUT_OF_RANGE;

		status = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (status != MEM_OK) return status;

		for (size_t i = 0; i < len; i++) {
			*(value + i) = (*((__IO uint8_t*) (realAddress + i)));
		}

		return MemoryOperationStatus::MEM_OK;
	}

	MemoryOperationStatus write(size_t virtualAddress, MemoryDriver<FlashCellSize>::Cell value) override {
		size_t realAddress;
		MemoryOperationStatus memStatus;

		if (value != 0 && !IsEmpty(virtualAddress)) return MEM_TAKEN;

		memStatus = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (memStatus != MEM_OK) return memStatus;

		if (HAL_FLASH_Unlock() != HAL_OK) return MEM_LOCKED;

		HAL_StatusTypeDef status =  HAL_FLASH_Program(	FLASH_TYPEPROGRAM_HALFWORD,
														realAddress,
														value);

		HAL_FLASH_Lock();

		return (status == HAL_OK) ? MEM_OK : MEM_ERROR;
	};

	MemoryOperationStatus fill(size_t virtualAddress, size_t cellsCount) override {
		MemoryOperationStatus memStatus;
		for (size_t i = 0; i < cellsCount; i++) {
			memStatus = write(virtualAddress + i * FlashCellSize, 0);
			if (memStatus != MEM_OK) return memStatus;
		}
		return MEM_OK;
	}

	MemoryOperationStatus findEmptySpace(size_t cellsCount, size_t* foundAddress) override {
		// MemoryOperationStatus memStatus;
		size_t address;
		for (size_t pageIndex = 0; pageIndex < memoryPagesCount; pageIndex++) {
//			MemoryPage page = memoryPages[pageIndex];
			for (address = 0; address < PageSize - cellsCount*FlashCellSize; address += FlashCellSize) {
				if (IsEmpty(address, cellsCount)) {
					*foundAddress = address;
					return MEM_OK;
				}
			}
		}
		return MEM_NOT_ENOUGH_SPACE;
	}

	MemoryOperationStatus clear(size_t virtualAddress) override {
		size_t realAddress;
		MemoryOperationStatus memStatus;

		memStatus = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (memStatus != MEM_OK) return memStatus;

		if (HAL_FLASH_Unlock() != HAL_OK) return MEM_LOCKED;

		HAL_StatusTypeDef status =  HAL_FLASH_Program(	FLASH_TYPEPROGRAM_HALFWORD,
														realAddress,
														0U);

		HAL_FLASH_Lock();

		return (status == HAL_OK) ? MEM_OK : MEM_ERROR;
	}

	bool IsEmpty(size_t virtualAddress) {
		size_t realAddress;

		MemoryOperationStatus memStatus = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		if (memStatus != MEM_OK) return false;

		for (size_t i = 0; i < FlashCellSize; i++) {
			if (*(((__IO uint8_t*)(realAddress)) + i) != 0xFF) return false;
		}

		return true;
	}

	bool IsEmpty(size_t virtualAddress, size_t cellsCount) override {
		for (size_t i = 0; i < cellsCount; i++) {
			if (!IsEmpty(virtualAddress + i*FlashCellSize)) return false;
		}
		return true;
	}

	bool IsPageEmpty(MemoryPage page) {
		if (!IsValidAddress(page.start)) return false;
		for (size_t address = page.start; address < page.end; address++) {
			if (~(*((__IO uint8_t*) address)) != 0) return false; 
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
template <size_t FlashCellSize>
size_t FlashMemoryDriver<FlashCellSize>::memoryPagesUsed = 0;

const size_t EmbeddedFlashCellSize = 2;
using EmbeddedFlashMemoryDriver = FlashMemoryDriver<EmbeddedFlashCellSize>;

//FlashMemoryDriver<>* getFlashDriver() {
//	static FlashMemoryDriver<>* flashDriver = new FlashMemoryDriver<>();
//	return flashDriver;
//}
//template <typename DataType = uint8_t>
//using FlashStorage = Storage<FlashMemoryDriver<>, DataType>;


}


#endif
