introsatlib_add_static_component(IntroSatLib_LIS2MDL LIS2MDL LIS2MDL.cpp)
target_link_libraries(
	IntroSatLib_LIS2MDL
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
