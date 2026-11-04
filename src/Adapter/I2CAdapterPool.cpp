/**
 * @file I2CAdapterPool.cpp
 * @brief I2CAdapter::pool_ — process-wide Adapter pool keyed by handle pointer.
 */
#define ISL_INTERNAL

#include "Adapter/I2C/I2CAdapter.h"

#ifdef ISL_I2C_ENABLED

#include <cstddef>

namespace IntroSatLib {
namespace adapters {

struct I2CAdapter::Pool {
	static constexpr std::size_t kSize = 8;

	struct Slot {
		I2C_HANDLE_TYPE* handle = nullptr;
		I2CAdapter adapter{nullptr};
		bool used = false;
	};

	Slot slots[kSize];

	I2C& acquire(I2C_HANDLE_TYPE* hi2c)
	{
		for (std::size_t i = 0; i < kSize; ++i) {
			if (slots[i].used && slots[i].handle == hi2c) {
				return slots[i].adapter;
			}
		}

		for (std::size_t i = 0; i < kSize; ++i) {
			if (!slots[i].used) {
				slots[i].adapter = I2CAdapter(hi2c);
				slots[i].handle = hi2c;
				slots[i].used = true;
				return slots[i].adapter;
			}
		}

		slots[0].adapter = I2CAdapter(hi2c);
		slots[0].handle = hi2c;
		slots[0].used = true;
		return slots[0].adapter;
	}
};

I2CAdapter::Pool I2CAdapter::pool_{};

I2C& I2CAdapter::from(I2C_HANDLE_TYPE* hi2c)
{
	return pool_.acquire(hi2c);
}

I2C& I2CAdapter::from(I2C_HANDLE_TYPE& hi2c)
{
	return from(&hi2c);
}

} /* namespace adapters */
} /* namespace IntroSatLib */

#endif /* ISL_I2C_ENABLED */
