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

template <typename Driver, size_t Size, typename DataType = uint8_t>
class Storage {
protected:
	constexpr size_t bytesSize = Size * sizeof(DataType);

	Driver* drv;
public:

	Storage(Driver* _drv): drv(_drv) {
	}

	virtual MemoryInitStatus Init() = 0;

	virtual MemoryOperationStatus ClearStorage() = 0;

	virtual MemoryOperationStatus get(size_t index, DataType* value) = 0;

	virtual MemoryOperationStatus store(size_t index, DataType value) = 0;

	virtual MemoryOperationStatus find(size_t index, size_t* address, bool* found) = 0; // make private
};


};



#endif /* MEMORY_STORAGE_H_ */
