introsatlib_add_static_component(IntroSatLib_ADS7830 ADS7830 ADS7830.cpp)
target_link_libraries(
	IntroSatLib_ADS7830
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
