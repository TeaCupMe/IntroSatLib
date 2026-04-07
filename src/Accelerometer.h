#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "AccelerometerV1.h"
#include "AccelerometerV2.h"

namespace IntroSatLib {

template <uint8_t version = 2>
class Accelerometer: public AccelerometerV2 {
	using AccelerometerV2::AccelerometerV2;
};

template <>
class Accelerometer<1>: public AccelerometerV1 {
	using AccelerometerV1::AccelerometerV1;
};

}

#endif /* ACCELEROMETER_H_ */
