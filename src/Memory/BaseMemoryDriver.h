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

// ISL_StatusTypeDef MemStatusToISLStatus(MemoryOperationStatus&& memStatus) {
// 	switch (memStatus) {
// 	case MEM_OK:
// 		return ISL_OK;
// 		break;

// 	case MEM_BUSY:
// 		return ISL_BUSY;
// 		break;

// 	case MEM_FULL:
// 	case MEM_OUT_OF_RANGE:
// 	case MEM_INVALID_ADDRESS:
// 	case MEM_LOCKED:
// 	case MEM_NOT_ENOUGH_SPACE:
// 	case MEM_ERROR:
// 	case MEM_TAKEN:
// 	default:
// 		return ISL_ERROR;
// 		break;
// 	};
// }

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
template <size_t CellSize>
struct __attribute__((__packed__)) Cell {
	uint8_t data[CellSize];
	explicit Cell(uint8_t value) {
		for (size_t i = 0; i < CellSize; i++) {
			data[i] = value;
		}
	}
	Cell() {};
	bool operator==(uint8_t value) {
		for (size_t i = 0; i < CellSize; i++) {
			if (data[i] != value) return false;
		}
		return true;
	}
	bool operator!=(uint8_t value) {
		for (size_t i = 0; i < CellSize; i++) {
			if (data[i] != value) return true;
		}
		return false;
	}
	bool operator!=(Cell<CellSize> lhs) {
		for (size_t i = 0; i < CellSize; i++) {
			if (data[i] != lhs.data[i]) return true;
		}
		return false;
	}
};


template <size_t WriteCellSize, size_t ReadCellSize = WriteCellSize>
class MemoryDriver {
	// using Cell = 
public:
	using WriteCell = Cell<WriteCellSize>;
	using ReadCell = Cell<ReadCellSize>;
	// struct __attribute__((__packed__)) WriteCell {
	// 	uint8_t data[WriteCellSize];

	// 	bool operator==(uint8_t value) {
	// 		for (size_t i = 0; i < WriteCellSize; i++) {
	// 			if (data[i] != value) return false;
	// 		}
	// 		return true;
	// 	}
	// 	bool operator!=(uint8_t value) {
	// 		for (size_t i = 0; i < WriteCellSize; i++) {
	// 			if (data[i] != value) return true;
	// 		}
	// 		return false;
	// 	}
	// };

	// struct __attribute__((__packed__)) ReadCell {
	// 	uint8_t data[ReadCellSize];
		
	// };

	const size_t CellSize = WriteCellSize;
	
	// TODO add state
public:
	virtual MemoryInitStatus Init(size_t size) = 0;
	virtual MemoryInitStatus Init(size_t dataTypeSize, size_t count) = 0;
	
	virtual MemoryOperationStatus read(size_t virtualAddress, ReadCell* value) = 0;
	virtual MemoryOperationStatus read(size_t virtualAddress, uint8_t* value, size_t len) = 0;
	virtual MemoryOperationStatus write(size_t virtualAddress, WriteCell value) = 0;
	virtual MemoryOperationStatus fill(size_t virtualAddress, size_t cellsCount) = 0;
	// virtual MemoryOperationStatus clear(size_t virtualAddress) = 0;
	virtual bool isEmpty(size_t virtualAddress) = 0;
	virtual bool isEmpty(size_t virtualAddress, size_t cellsCount) = 0;
	virtual MemoryOperationStatus EraseAll() = 0;
	virtual MemoryOperationStatus findEmptySpace(size_t cellsCount, size_t* foundAddress) = 0;

};

};



#endif /* MEMORYDRIVER_H_ */
