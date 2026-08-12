cmake_minimum_required(VERSION 3.20)

project(
	IntroSatLib
	VERSION 0.2.0
	DESCRIPTION "Library for the IntroSat educational satellite kit"
	LANGUAGES CXX
)

include(GNUInstallDirs)

set(
	INTROSATLIB_BACKEND
	"STM32_HAL"
	CACHE STRING
	"Platform backend: ARDUINO or STM32_HAL"
)
set_property(CACHE INTROSATLIB_BACKEND PROPERTY STRINGS ARDUINO STM32_HAL)

set(
	INTROSATLIB_PLATFORM_TARGET
	""
	CACHE STRING
	"Existing CMake target that provides the selected platform framework"
)
option(INTROSATLIB_BUILD_DOCS "Add the Doxygen documentation target" OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(_introsatlib_install_default ON)
else()
	set(_introsatlib_install_default OFF)
endif()
option(
	INTROSATLIB_ENABLE_INSTALL
	"Generate install rules and a find_package configuration"
	${_introsatlib_install_default}
)
unset(_introsatlib_install_default)

string(TOUPPER "${INTROSATLIB_BACKEND}" INTROSATLIB_BACKEND)
set(_introsatlib_supported_backends ARDUINO STM32_HAL)
if(NOT INTROSATLIB_BACKEND IN_LIST _introsatlib_supported_backends)
	message(
		FATAL_ERROR
		"Unsupported INTROSATLIB_BACKEND='${INTROSATLIB_BACKEND}'. "
		"Expected one of: ARDUINO, STM32_HAL."
	)
endif()

if(
	INTROSATLIB_PLATFORM_TARGET STREQUAL ""
	AND INTROSATLIB_BACKEND STREQUAL "STM32_HAL"
	AND TARGET stm32cubemx
)
	set(INTROSATLIB_PLATFORM_TARGET stm32cubemx)
endif()

if(INTROSATLIB_PLATFORM_TARGET STREQUAL "")
	message(
		FATAL_ERROR
		"The ${INTROSATLIB_BACKEND} backend requires a platform target. "
		"Set INTROSATLIB_PLATFORM_TARGET to a target created by the parent "
		"firmware project. STM32_HAL automatically uses 'stm32cubemx' when "
		"that target exists."
	)
endif()
if(NOT TARGET "${INTROSATLIB_PLATFORM_TARGET}")
	message(
		FATAL_ERROR
		"Platform target '${INTROSATLIB_PLATFORM_TARGET}' does not exist. "
		"Create it before adding IntroSatLib."
	)
endif()

add_library(IntroSatLib::Platform INTERFACE IMPORTED)
set_property(
	TARGET IntroSatLib::Platform
	PROPERTY INTERFACE_LINK_LIBRARIES "${INTROSATLIB_PLATFORM_TARGET}"
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

if(INTROSATLIB_ENABLE_INSTALL)
	include(CMakePackageConfigHelpers)
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
endif()
