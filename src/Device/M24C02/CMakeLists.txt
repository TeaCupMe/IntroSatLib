introsatlib_add_static_component(IntroSatLib_M24C02 M24C02 M24C02.cpp)
target_link_libraries(
	IntroSatLib_M24C02
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
