introsatlib_add_static_component(IntroSatLib_LSM6DS3 LSM6DS3 LSM6DS3.cpp)
target_link_libraries(
	IntroSatLib_LSM6DS3
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
