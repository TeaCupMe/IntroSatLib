#ifndef BASE_MEMORYDRIVER_H_
#define BASE_MEMORYDRIVER_H_

#include "cstdint"

namespace IntroSatLib
{

class BaseMemoryDriver
{
protected:
	uint32_t memorySize = 0;
public:
	virtual ISL_StatusTypeDef Read(uint32_t addr, uint8_t* data, uint16_t len) = 0;
	virtual ISL_StatusTypeDef Write(uint32_t addr, uint8_t* data, uint16_t len) = 0;

	virtual ISL_StatusTypeDef FullErase() = 0;

	template <typename T>
	ISL_StatusTypeDef Save(uint32_t addr, T* t)
	{
		return Write(addr, (uint8_t*)t, sizeof(T));
	}

	template <typename T>
	ISL_StatusTypeDef Load(uint32_t addr, T* t)
	{
		return Read(addr, (uint8_t*)t, sizeof(T));
	}
};

}




#endif /* BASE_MEMORYDRIVER_H_ */
