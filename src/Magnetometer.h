#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "AK8963.h"
#include "MagnetometerV2.h"
#include "MagnetometerV3.h"

namespace IntroSatLib {

template <uint8_t version = 2>
class Magnetometer: public MagnetometerV2 {
	using MagnetometerV2::MagnetometerV2;
};

template <>
class Magnetometer<1>: public AK8963 {
	using AK8963::AK8963;
};

template <>
class Magnetometer<3>: public MagnetometerV3 {
	using MagnetometerV3::MagnetometerV3;
};

}

#endif /* MAGNETOMETER_H_ */
