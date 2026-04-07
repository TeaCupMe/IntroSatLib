#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "GyroscopeV1.h"
#include "GyroscopeV2.h"

namespace IntroSatLib {

template <uint8_t version = 2>
class Gyroscope: public GyroscopeV2 {
	using GyroscopeV2::GyroscopeV2;
};

template <>
class Gyroscope<1>: public GyroscopeV1 {
	using GyroscopeV1::GyroscopeV1;
};

}

#endif /* GYROSCOPE_H_ */
