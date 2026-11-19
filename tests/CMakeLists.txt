include(FetchContent)

# Match the library's C++17 requirement when configuring GoogleTest itself.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
if(MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(
	introsatlib_unit_tests
	unit/test_harness_smoke.cpp
	unit/test_I2CDevice.cpp
	unit/test_I2CDriverLink.cpp
	unit/test_LM75A.cpp
	unit/test_LSM6DS3.cpp
	# Strong LOGGER → stderr for driver logs.
	# Must not live only in libIntroSatLib_Core.a (weak LOGGER in Logger.cpp wins).
	fakes/HostLogger.cpp
)

target_compile_features(introsatlib_unit_tests PRIVATE cxx_std_17)
target_link_libraries(
	introsatlib_unit_tests
	PRIVATE
		GTest::gmock_main
		IntroSatLib::LM75A
		IntroSatLib::LSM6DS3
		IntroSatLib::Facade
		IntroSatLib::ADS7830
		IntroSatLib::FlyWheel
		IntroSatLib::LIS2MDL
		IntroSatLib::LIS3MDL
		IntroSatLib::MS5611
		IntroSatLib::Device
		IntroSatLib::Core
)

include(GoogleTest)
gtest_discover_tests(introsatlib_unit_tests NO_PRETTY_VALUES)

# Exercise firmware constructors and the real Arduino adapter separately from
# MockI2C tests. Only the Arduino framework is replaced by small test doubles.
add_executable(introsatlib_i2c_platform_tests
	platform/test_I2CAdapterPool.cpp
	../src/Adapter/I2C/ARDUINO_I2C.cpp
	../src/Adapter/ARDUINO/ARDUINO_UART.cpp
	../src/Device/I2CDevice.cpp
	../src/Logger.cpp
	fakes/HostSystem.cpp
)
target_compile_features(introsatlib_i2c_platform_tests PRIVATE cxx_std_17)
target_compile_definitions(introsatlib_i2c_platform_tests PRIVATE ARDUINO)
target_include_directories(introsatlib_i2c_platform_tests PRIVATE
	platform/arduino
	../src
)
target_link_libraries(introsatlib_i2c_platform_tests PRIVATE GTest::gtest_main)
gtest_discover_tests(introsatlib_i2c_platform_tests NO_PRETTY_VALUES)

# The existing build preset builds this target; include the platform checks.
add_dependencies(introsatlib_unit_tests introsatlib_i2c_platform_tests)
