#ifndef INTROSATLIB_DEF_H_
#define INTROSATLIB_DEF_H_

#include <stdint.h>



#ifdef USE_HAL_DRIVER
enum ISL_StatusTypeDef: uint8_t {
	ISL_OK      = 0x00U,
	ISL_ERROR	= 0x01U,
	ISL_BUSY	= 0x02U,
	ISL_TIMEOUT	= 0x03U
};
#else
enum ISL_StatusTypeDef: uint8_t {
	ISL_OK      = 0x00U,
	ISL_ERROR	= 0x01U,
	ISL_BUSY	= 0x02U,
	ISL_TIMEOUT	= 0x03U
};
#endif

#define RETURN_STATUS_IF_NOT_OK(func, status) if ((status = func) != ISL_StatusTypeDef::ISL_OK) { return status; }




#endif /* INTROSATLIB_DEF_H_ */
