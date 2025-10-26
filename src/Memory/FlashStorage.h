/*
 * FlashStorage.h
 *
 *  Created on: Oct 25, 2025
 *      Author: alesh
 */

#ifndef MEMORY_FLASHSTORAGE_H_
#define MEMORY_FLASHSTORAGE_H_

#include "Memory/Storage.h"
#include "Memory/FlashMemoryDriver.h"
namespace IntroSatLib::memory {


//template <typename DataType = uint8_t>
//class FlashStorage: protected Storage<FlashMemoryDriver<>, DataType> {
//public:
//	FlashMemoryDriver<>* drv;
//	FlashStorage(FlashMemoryDriver<> flashStorage) {
//		this->drv = &flashStorage;
//	}
//
//	MemoryInitStatus Init(size_t _size) {
//		MemoryInitStatus status;
//		status = Storage<FlashMemoryDriver<>, DataType>::Init(_size);
//		if (status != MEM_INIT_OK) return status;
//		return this->drv->Init(this->bytesSize);
//	}
//
//	MemoryOperationStatus ClearMemory() override {
//		return this->drv->EraseAll();
//	}
//
//	MemoryOperationStatus get(size_t index, DataType* value) override {
//		return MEM_OK;
//	}
//};
}
//


#endif /* MEMORY_FLASHSTORAGE_H_ */
