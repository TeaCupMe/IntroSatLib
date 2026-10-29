cmake_minimum_required(VERSION 3.20)
project(IntroSatLibAddSubdirectorySmoke LANGUAGES CXX)

if(INTROSATLIB_BACKEND STREQUAL "ARDUINO")
	add_library(smoke_platform INTERFACE)
	set(INTROSATLIB_PLATFORM_TARGET smoke_platform CACHE STRING "" FORCE)
else()
	# Exercises automatic STM32 platform-target detection.
	add_library(stm32cubemx INTERFACE)
endif()
set(INTROSATLIB_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(
	"${CMAKE_CURRENT_LIST_DIR}/../../.."
	"${CMAKE_CURRENT_BINARY_DIR}/introsatlib"
)

set(
	_expected_targets
	IntroSatLib::IntroSatLib
	IntroSatLib::Core
	IntroSatLib::Device
	IntroSatLib::Facade
	IntroSatLib::Base
	IntroSatLib::ADS7830
	IntroSatLib::CC1101
	IntroSatLib::E32_433
	IntroSatLib::FlyWheel
	IntroSatLib::FSC_BT986
	IntroSatLib::IRCamera
	IntroSatLib::ISLIRProtocol
	IntroSatLib::ISLIRReceiver
	IntroSatLib::ISLIRTransmitter
	IntroSatLib::ISLIRTransceiver
	IntroSatLib::LIS2MDL
	IntroSatLib::LIS3MDL
	IntroSatLib::LM75A
	IntroSatLib::LSM6DS3
	IntroSatLib::M24C02
	IntroSatLib::MS5611
)
foreach(_target IN LISTS _expected_targets)
	if(NOT TARGET "${_target}")
		message(FATAL_ERROR "Expected target '${_target}' was not created")
	endif()
endforeach()
