cmake_minimum_required(VERSION 3.20)

project(
	IntroSatLib
	VERSION 0.2.0
	DESCRIPTION "Library for the IntroSat educational satellite kit"
	LANGUAGES CXX
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(
	INTROSATLIB_BACKEND
	"STM32_HAL"
	CACHE STRING
	"Platform backend: ARDUINO or STM32_HAL"
)
set_property(CACHE INTROSATLIB_BACKEND PROPERTY STRINGS ARDUINO STM32_HAL)

set(
	INTROSATLIB_ARDUINO_TARGET
	""
	CACHE STRING
	"Existing CMake target that provides Arduino Core, Wire and SPI"
)
set(
	INTROSATLIB_STM32_HAL_TARGET
	""
	CACHE STRING
	"Existing CMake target that provides STM32 HAL and CMSIS"
)
option(INTROSATLIB_BUILD_DOCS "Add the Doxygen documentation target" OFF)

string(TOUPPER "${INTROSATLIB_BACKEND}" INTROSATLIB_BACKEND)
set(_introsatlib_supported_backends ARDUINO STM32_HAL)
if(NOT INTROSATLIB_BACKEND IN_LIST _introsatlib_supported_backends)
	message(
		FATAL_ERROR
		"Unsupported INTROSATLIB_BACKEND='${INTROSATLIB_BACKEND}'. "
		"Expected one of: ARDUINO, STM32_HAL."
	)
endif()

if(INTROSATLIB_BACKEND STREQUAL "ARDUINO")
	set(INTROSATLIB_FRAMEWORK_TARGET "${INTROSATLIB_ARDUINO_TARGET}")
else()
	set(INTROSATLIB_FRAMEWORK_TARGET "${INTROSATLIB_STM32_HAL_TARGET}")
endif()

if(INTROSATLIB_FRAMEWORK_TARGET STREQUAL "")
	message(
		FATAL_ERROR
		"The ${INTROSATLIB_BACKEND} backend requires a framework target. "
		"Set INTROSATLIB_${INTROSATLIB_BACKEND}_TARGET to a target created "
		"by the parent firmware project."
	)
endif()
if(NOT TARGET "${INTROSATLIB_FRAMEWORK_TARGET}")
	message(
		FATAL_ERROR
		"Framework target '${INTROSATLIB_FRAMEWORK_TARGET}' does not exist. "
		"Create it before adding IntroSatLib."
	)
endif()

add_library(IntroSatLib::Framework INTERFACE IMPORTED)
set_property(
	TARGET IntroSatLib::Framework
	PROPERTY INTERFACE_LINK_LIBRARIES "${INTROSATLIB_FRAMEWORK_TARGET}"
)

add_subdirectory(src)

if(INTROSATLIB_BUILD_DOCS)
	find_package(Doxygen REQUIRED COMPONENTS doxygen dot)
	add_custom_target(
		docs
		COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		COMMENT "Generating IntroSatLib API documentation"
		VERBATIM
	)
endif()

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/IntroSatLibConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/IntroSatLibConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/IntroSatLib"
)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/IntroSatLibConfigVersion.cmake"
	COMPATIBILITY SameMajorVersion
)

install(
	EXPORT IntroSatLibTargets
	FILE IntroSatLibTargets.cmake
	NAMESPACE IntroSatLib::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/IntroSatLib"
)
install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/IntroSatLibConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/IntroSatLibConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/IntroSatLib"
)
