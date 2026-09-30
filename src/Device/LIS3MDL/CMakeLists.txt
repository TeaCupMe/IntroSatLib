introsatlib_add_static_component(IntroSatLib_LIS3MDL LIS3MDL LIS3MDL.cpp)
target_link_libraries(
	IntroSatLib_LIS3MDL
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
