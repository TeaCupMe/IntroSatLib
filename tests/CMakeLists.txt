include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(
	introsatlib_unit_tests
	unit/test_harness_smoke.cpp
	unit/test_I2CDevice.cpp
	unit/test_LM75A.cpp
	unit/test_LSM6DS3.cpp
)

target_compile_features(introsatlib_unit_tests PRIVATE cxx_std_17)
target_link_libraries(
	introsatlib_unit_tests
	PRIVATE
		Catch2::Catch2WithMain
		IntroSatLib::LM75A
		IntroSatLib::LSM6DS3
		IntroSatLib::Device
		IntroSatLib::Core
)

list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(introsatlib_unit_tests)
