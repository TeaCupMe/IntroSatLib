/*
 * BaseMemory.h
 *
 *  Created on: Oct 25, 2025
 *      Author: alesh
 */

#ifndef MEMORYDRIVER_H_
#define MEMORYDRIVER_H_

#include <stdint.h>
#include "IntroSatLib_def.h"
#include "Adapter/System.h"

namespace IntroSatLib::memory {

enum MemoryOperationStatus {
	MEM_OK,
	MEM_BUSY,
	MEM_FULL,
	MEM_OUT_OF_RANGE,
	MEM_INVALID_ADDRESS,
	MEM_LOCKED,
	MEM_TAKEN,
	MEM_ERROR,
	MEM_NOT_ENOUGH_SPACE,
};

ISL_StatusTypeDef MemStatusToISLStatus(MemoryOperationStatus&& memStatus) {
	switch (memStatus) {
	case MEM_OK:
		return ISL_OK;
		break;

	case MEM_BUSY:
		return ISL_BUSY;
		break;

	case MEM_FULL:
	case MEM_OUT_OF_RANGE:
	case MEM_INVALID_ADDRESS:
	case MEM_LOCKED:
	case MEM_NOT_ENOUGH_SPACE:
	case MEM_ERROR:
	case MEM_TAKEN:
	default:
		return ISL_ERROR;
		break;
	}
}

enum MemoryInitStatus {
	MEM_INIT_OK,
	MEM_INIT_NOT_ENOUGH_SPACE,
	MEM_INIT_REPEAT,
	MEM_INIT_ERROR

};

// enum MemoryCellSize {
// 	MEM_CELL_1B = 1,
// 	MEM_CELL_2B = 2,
// 	MEM_CELL_4B = 4
// };

template <size_t CellSizeBytes>
class MemoryDriver {
	// using Cell = 
public:
	typedef struct __attribute__((__packed__)) {
		uint8_t data[CellSizeBytes];
	} Cell;

	const size_t CellSize = CellSizeBytes;
	
	// TODO add state
public:
	virtual MemoryInitStatus Init(size_t size) = 0;
	
	virtual MemoryOperationStatus read(size_t local_addr, Cell* value) = 0;
	virtual MemoryOperationStatus read(size_t local_addr, uint8_t* value, size_t len) = 0;
	virtual MemoryOperationStatus write(size_t local_addr, Cell value) = 0;
	virtual MemoryOperationStatus fill(size_t local_addr, size_t cellsCount) = 0;
	virtual MemoryOperationStatus clear(size_t local_addr) = 0;
	virtual bool IsEmpty(size_t local_addr) = 0;
	virtual bool IsEmpty(size_t local_addr, size_t cellsCount) = 0;
	virtual MemoryOperationStatus EraseAll() = 0;
	virtual MemoryOperationStatus findEmptySpace(size_t cellsCount, size_t* foundAddress) = 0;

};

};



#endif /* MEMORYDRIVER_H_ */
