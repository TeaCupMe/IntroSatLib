/**
 * @file I2CAdapterPool.cpp
 * @brief Process-wide Adapter pool keyed by handle pointer.
 */
#define ISL_INTERNAL

#include "Adapter/I2C/I2CAdapter.h"

#ifdef ISL_I2C_ENABLED

#include <stddef.h>

namespace IntroSatLib {
namespace interfaces {

struct I2CAdapter::Pool {
	static constexpr size_t kSize = 8;

	struct Slot {
		I2C_HANDLE_TYPE* handle = nullptr;
		I2CAdapter adapter{nullptr};
		bool used = false;
	};

	Slot slots[kSize];
	I2CAdapter unavailable{nullptr};

	I2C& acquire(I2C_HANDLE_TYPE* hi2c)
	{
		if (hi2c == nullptr) {
			return unavailable;
		}

		for (size_t i = 0; i < kSize; ++i) {
			if (slots[i].used && slots[i].handle == hi2c) {
				return slots[i].adapter;
			}
		}

		for (size_t i = 0; i < kSize; ++i) {
			if (!slots[i].used) {
				slots[i].adapter = I2CAdapter(hi2c);
				slots[i].handle = hi2c;
				slots[i].used = true;
				return slots[i].adapter;
			}
		}

		// Existing devices retain references to their adapters for their lifetime.
		return unavailable;
	}
};

I2C& I2CAdapter::from(I2C_HANDLE_TYPE* hi2c)
{
	// Global device constructors may call from() before other translation units
	// have initialized their globals. Construct the pool on its first use.
	static Pool pool;
	return pool.acquire(hi2c);
}

I2C& I2CAdapter::from(I2C_HANDLE_TYPE& hi2c)
{
	return from(&hi2c);
}

} /* namespace interfaces */
} /* namespace IntroSatLib */

#endif /* ISL_I2C_ENABLED */
