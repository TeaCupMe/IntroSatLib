#ifndef FLASHMEMORYDRIVER_H_
#define FLASHMEMORYDRIVER_H_

#include "Memory/BaseMemoryDriver.h"

/*
 * General Flash properties
 * 	- Pagination. Page - minimal erasable unit
 * 	- Read granularity.
 * 	- Write granularity. Data can be written only with some granularity
 ? 	- Write can happen only to blank cell.
 *
 *
 * */

namespace IntroSatLib::memory {

template <size_t WriteGranularity, size_t ReadGranularity, size_t PageSize, size_t PageCount, size_t FirstPageAddressOffset = 0, uint8_t BlankValue = 0xFF>
class FlashMemoryDriver: MemoryDriver<WriteGranularity, ReadGranularity> {
protected: 
	using ReadCell = typename MemoryDriver<WriteGranularity, ReadGranularity>::ReadCell;
	using WriteCell = typename MemoryDriver<WriteGranularity, ReadGranularity>::WriteCell;
	const ReadCell emptyCell{0xFF};


	struct FlashAddress {
		size_t address;
		explicit FlashAddress(const size_t _address): address(_address) {};
		FlashAddress() {};
	};

	struct MemoryPage {
		size_t startAddress;
		size_t endAddress;
//		uint64_t writeCycles;
//		uint64_t maxWriteCycles = UINT64_MAX; //TODO use something real
	};
	MemoryPage* memoryPages;
	size_t memoryPagesCount = 0;

	bool IsValidReadAddress(size_t virtualAddress) {
		if ((virtualAddress % ReadGranularity) != 0) return false;
		if (virtualAddress >= memoryPagesCount * PageSize) return false;

		return true;
	}

	bool IsValidWriteAddress(size_t virtualAddress) {
		if ((virtualAddress % WriteGranularity) != 0) return false;
		if (virtualAddress >= memoryPagesCount * PageSize) return false;

		return true;
	}

	bool IsValidPageStartAddress(size_t virtualAddress) {
		if ((virtualAddress % PageSize) != 0) return false;

		return true;
	}

	bool IsValidAddress(size_t virtualAddress) {
		return IsValidWriteAddress(virtualAddress) || IsValidReadAddress(virtualAddress);
	}

	virtual bool IsValidFlashAddress(FlashAddress flashAddress) {
		if (flashAddress.address < FirstPageAddressOffset) return false;
		if (flashAddress.address >= FirstPageAddressOffset + memoryPagesCount * PageSize) return false;
		return true;
	}

	MemoryOperationStatus VirtualAddressToFlashAddress(size_t virtualAddress, FlashAddress* flashAddress) {
		if (!IsValidAddress(virtualAddress)) return MEM_OUT_OF_RANGE;
		size_t pageIndex = virtualAddress / PageSize;
		size_t addrOnPage = (virtualAddress % PageSize);
		*flashAddress = FlashAddress(memoryPages[pageIndex].startAddress + addrOnPage);
		return MEM_OK;
	}

	// Written by GPT-4
	MemoryOperationStatus FlashAddressToVirtualAddress(FlashAddress flashAddress, size_t* virtualAddress) {
		if (!IsValidFlashAddress(flashAddress)) return MEM_OUT_OF_RANGE;
		for (size_t i = 0; i < memoryPagesCount; i++) {
			if ((flashAddress.address >= memoryPages[i].startAddress) && (flashAddress.address <= memoryPages[i].endAddress)) {
				*virtualAddress = (i * PageSize) + (flashAddress.address - memoryPages[i].startAddress);
				return MEM_OK;
			}
		}
		return MEM_OUT_OF_RANGE;
	}

	virtual MemoryOperationStatus _read(FlashAddress flashAddress, ReadCell* value) = 0;

	virtual MemoryOperationStatus _read(FlashAddress flashAddress, uint8_t* value, size_t len) = 0;

	virtual MemoryOperationStatus _write(FlashAddress flashAddress, WriteCell value) = 0;

	virtual MemoryOperationStatus ErasePage(MemoryPage page) = 0;

	virtual MemoryOperationStatus unlockFlash() = 0;
	virtual MemoryOperationStatus lockFlash() = 0;

	virtual bool isEmpty(size_t virtualAddress) override {
		ReadCell readCell;

		if (!IsValidReadAddress(virtualAddress)) return false;
		if (read(virtualAddress, &readCell) != MEM_OK) return false;
		if (readCell != emptyCell) return false;

		return true;
	};
	
	virtual bool isEmpty(size_t virtualAddress, size_t cellsCount) override {
		for (size_t i = 0; i < cellsCount; i ++) {
			if (!isEmpty(virtualAddress + i * ReadGranularity)) return false;
		}
		return true;
	}

	bool isPageEmpty(MemoryPage page) {
		if (!IsValidPageStartAddress(page.start)) return false; //? unnessecary

		for (size_t address = page.start; address < page.end; address += ReadGranularity) {
			if (!isEmpty(address)) return false;
		}
		return true;
	}

public:
	MemoryInitStatus Init(size_t dataTypeSize, size_t count) override {
		size_t alignedDataTypeSize = ((dataTypeSize + WriteGranularity - 1) / WriteGranularity) * WriteGranularity; // Align dataTypeSize to WriteGranularity 
		return Init(alignedDataTypeSize * count);
	}

	MemoryInitStatus Init(size_t size) override {
		if (memoryPagesCount > 0) return MEM_INIT_REPEAT;
		memoryPagesCount = (size + PageSize - 1) / PageSize;
		if (memoryPagesCount > PageCount) return MEM_INIT_NOT_ENOUGH_SPACE;

		memoryPages = new MemoryPage[memoryPagesCount];

		for (uint8_t i = 0; i < memoryPagesCount; i++) {
			memoryPages[i].startAddress = FirstPageAddressOffset + (PageCount - i - 1) * PageSize;
			memoryPages[i].endAddress = FirstPageAddressOffset + (PageCount - i) * PageSize - 1;
		}

		return MEM_INIT_OK;
	}

	MemoryOperationStatus read(size_t virtualAddress, ReadCell* value) override {
		MemoryOperationStatus memStatus;
		FlashAddress flashAddress;

		if (!(IsValidReadAddress(virtualAddress))) return MEM_OUT_OF_RANGE;

		memStatus = VirtualAddressToFlashAddress(virtualAddress, &flashAddress);
		if (memStatus != MEM_OK) return memStatus;

		return _read(flashAddress, value); // call internal method
	}

	MemoryOperationStatus read(size_t virtualAddress, uint8_t* value, size_t len) override {
		MemoryOperationStatus memStatus;
		FlashAddress flashAddress;

		// Check address of the beginning
		if (!(IsValidReadAddress(virtualAddress))) return MEM_OUT_OF_RANGE;

		// Check address of the end
		if (!(IsValidReadAddress(virtualAddress + len))) return MEM_OUT_OF_RANGE;

		memStatus = VirtualAddressToFlashAddress(virtualAddress, &flashAddress);
		if (memStatus != MEM_OK) return memStatus;

		return _read(flashAddress, value, len);
	}

	MemoryOperationStatus write(size_t virtualAddress, WriteCell value) override {
		MemoryOperationStatus memStatus;
		FlashAddress flashAddress;

		if (!IsValidWriteAddress(virtualAddress)) return MEM_INVALID_ADDRESS;

		memStatus = VirtualAddressToFlashAddress(virtualAddress, &flashAddress);
		if (memStatus != MEM_OK) return memStatus;

		memStatus = unlockFlash();
		if (memStatus != MEM_OK) return memStatus;

		memStatus = _write(flashAddress, value);

		lockFlash();

		return memStatus;
	}

	MemoryOperationStatus fill(size_t virtualAddress, size_t cellsCount) override {
		MemoryOperationStatus memStatus;
		for (size_t i = 0; i < cellsCount; i++) {
			memStatus = write(virtualAddress + i * WriteGranularity, WriteCell(0U));
			if (memStatus != MEM_OK) return memStatus;
		}
		return MEM_OK;
	}

	MemoryOperationStatus clear(size_t virtualAddress) {
		// size_t realAddress;
		// MemoryOperationStatus memStatus;

		// memStatus = VirtualAddressToRealAddress(virtualAddress, &realAddress);
		// if (memStatus != MEM_OK) return memStatus;

		// if (HAL_FLASH_Unlock() != HAL_OK) return MEM_LOCKED;

		return write(virtualAddress, WriteCell(0));

		// HAL_StatusTypeDef status =  HAL_FLASH_Program(	FLASH_TYPEPROGRAM_HALFWORD,
		// 												realAddress,
		// 												0U);

		// HAL_FLASH_Lock();

		// return (status == HAL_OK) ? MEM_OK : MEM_ERROR;
	}

	MemoryOperationStatus findEmptySpace(size_t cellsCount, size_t* foundAddress) override {
		// MemoryOperationStatus memStatus;
		size_t address;
		for (size_t pageIndex = 0; pageIndex < memoryPagesCount; pageIndex++) {
//			MemoryPage page = memoryPages[pageIndex];
			for (address = 0; address < PageSize - cellsCount * WriteGranularity; address += WriteGranularity) {
				if (isEmpty(address, cellsCount)) {
					*foundAddress = address;
					return MEM_OK;
				}
			}
		}
		return MEM_NOT_ENOUGH_SPACE;
	}

	MemoryOperationStatus EraseAll() override {
		MemoryOperationStatus memStatus;
		for (size_t pageIndex = 0; pageIndex < memoryPagesCount; pageIndex++) {
			memStatus = ErasePage(memoryPages[pageIndex]);
			if (memStatus != MEM_OK) return memStatus;
		}
		return MEM_OK;
	}

};

};

#endif
