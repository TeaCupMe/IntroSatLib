#ifndef INTROSATLIB_DEF_H_
#define INTROSATLIB_DEF_H_

#include <stdint.h>

enum ISL_StatusTypeDef: uint8_t {
	ISL_OK      = 0x00U,
	ISL_ERROR	= 0x01U,
	ISL_BUSY	= 0x02U,
	ISL_TIMEOUT	= 0x03U
};





#endif /* INTROSATLIB_DEF_H_ */
