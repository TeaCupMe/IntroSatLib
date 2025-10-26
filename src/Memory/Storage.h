/*
 * Storage.h
 *
 *  Created on: Oct 25, 2025
 *      Author: alesh
 */

#ifndef MEMORY_STORAGE_H_
#define MEMORY_STORAGE_H_

#include "Memory/BaseMemoryDriver.h"

namespace IntroSatLib::memory {

template <size_t Size, typename DataType = uint8_t>
class Storage {
protected:
//	__attribute__((__packed__))
	struct __attribute__((__packed__)) StoredDataType {
		uint16_t key;
		uint16_t valid;
		uint16_t length = sizeof(DataType);
		DataType data;
	};

	size_t bytesSize = Size * sizeof(DataType);

	MemoryDriver* drv;
public:

	Storage(MemoryDriver* _drv) {
		this->drv = _drv;
	}

	MemoryInitStatus Init() {
		return this->drv->Init(this->bytesSize);
	}

	MemoryOperationStatus ClearStorage() {
		return this->drv->EraseAll();
	}

	MemoryOperationStatus get(size_t index, DataType* value) {
		return MEM_OK;
	}

	MemoryOperationStatus set(size_t index, DataType* value) {
		return MEM_OK;
	}
};


};



#endif /* MEMORY_STORAGE_H_ */
