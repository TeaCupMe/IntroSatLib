introsatlib_add_static_component(IntroSatLib_MS5611 MS5611 MS5611.cpp)
target_link_libraries(
	IntroSatLib_MS5611
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
