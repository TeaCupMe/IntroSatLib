/*
 * FlashStorage.h
 *
 *  Created on: Oct 25, 2025
 *      Author: alesh
 */

#ifndef MEMORY_EMBEDDEDFLASHSTORAGE_H_
#define MEMORY_EMBEDDEDFLASHSTORAGE_H_

#include "Memory/Storage.h"
#include "Memory/FlashMemoryDriver.h"
namespace IntroSatLib::memory {


template <size_t Size, typename DataType = uint8_t>
class EmbeddedFlashStorage: protected Storage<EmbeddedFlashMemoryDriver, Size, DataType> {
protected:
	struct StoredDataType {
        uint16_t key;
		uint16_t length = sizeof(DataType) + 4;
        DataType data;
	};
    // __attribute__((__packed__));

    const size_t StoredDataTypeSizeInCells = ((sizeof(StoredDataType) + sizeof(EmbeddedFlashCellSize) - 1) / sizeof(EmbeddedFlashCellSize));

	size_t bytesSize = Size * sizeof(DataType);

	EmbeddedFlashMemoryDriver* drv;

    MemoryOperationStatus invalidate(size_t key) {
        bool found = false;
        size_t address;
        MemoryOperationStatus memStatus;
        do {
            memStatus = find(key, &address, &found);
            if (memStatus != MEM_OK) {
                return memStatus; 
            }
            if (found) {
                memStatus = this->drv->fill(address, StoredDataTypeSizeInCells);
                if (memStatus != MEM_OK) return memStatus;
            }
        } while (found);
        return MEM_OK;

    }
public:
	
	EmbeddedFlashStorage(EmbeddedFlashMemoryDriver* flashDriver): Storage<EmbeddedFlashMemoryDriver, Size, DataType>(flashDriver) {

	}

	MemoryInitStatus Init() override {
		return this->drv->Init(bytesSize);
	}

	MemoryOperationStatus ClearStorage() override {
		return this->drv->EraseAll();
	}

	MemoryOperationStatus get(size_t index, DataType* value) override {
		return MEM_OK;
	}

    MemoryOperationStatus find(size_t key, size_t* foundAddress, bool* found) override {
        size_t address = 0;
        MemoryOperationStatus status = MEM_OK;
        uint16_t _key = 0;

        while (address < bytesSize - sizeof(StoredDataType)) {
            
            status = this->drv->read(address, (uint8_t*) &_key, sizeof(_key));

            if (status != MEM_OK && status != MEM_OUT_OF_RANGE) {
                *found = false;
                *foundAddress = 0;
                return status; 
            }

            if (_key == 0) {
                address += sizeof(EmbeddedFlashCellSize);
                continue;
            } else {
                if (_key == key) {
                    *found = true;
                    *foundAddress = address;
                    return MEM_OK;
                } 

                uint16_t size = 0;
                status = this->drv->read(address + sizeof(_key), &size);

                if (status != MEM_OK) {
                    *found = false;
                    address = 0;
                    return status; 
                }

                address += StoredDataTypeSizeInCells;
                continue;
            }
        }
        *found = false;
        *foundAddress = 0;
        return MEM_OK;
    }

    MemoryOperationStatus store(size_t key, DataType value) override {
        if (key == 0) return MEM_INVALID_ADDRESS;
        StoredDataType stored;
        MemoryOperationStatus memStatus;
        size_t address;

        stored.key = key;
        stored.length = sizeof(DataType);
        stored.data = value;

        memStatus = invalidate(key);
        if (memStatus != MEM_OK) return memStatus;

        memStatus = this->drv->findEmptySpace(StoredDataTypeSizeInCells, &address);

        for (size_t i = 0; i < StoredDataTypeSizeInCells; i++) {
            memStatus = this->drv->write(address + i*sizeof(EmbeddedFlashCellSize), *(((EmbeddedFlashCellSize*)&stored)+i));
            if (memStatus != MEM_OK) return memStatus;
        }

		return MEM_OK;
	}
};
}
//


#endif /* MEMORY_EMBEDDEDFLASHSTORAGE_H_ */
