set(INTROSATLIB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

if(INTROSATLIB_BACKEND STREQUAL "ARDUINO")
	set(
		INTROSATLIB_ADAPTER_SOURCES
		Adapter/ARDUINO/ARDUINO_GPIO.cpp
		Adapter/ARDUINO/ARDUINO_I2C.cpp
		Adapter/ARDUINO/ARDUINO_SPI.cpp
		Adapter/ARDUINO/ARDUINO_System.cpp
		Adapter/ARDUINO/ARDUINO_UART.cpp
	)
	set(INTROSATLIB_BACKEND_DEFINITION ARDUINO)
else()
	set(
		INTROSATLIB_ADAPTER_SOURCES
		Adapter/STM32/STM32_GPIO.cpp
		Adapter/STM32/STM32_I2C.cpp
		Adapter/STM32/STM32_SPI.cpp
		Adapter/STM32/STM32_System.cpp
		Adapter/STM32/STM32_UART.cpp
	)
	set(INTROSATLIB_BACKEND_DEFINITION USE_HAL_DRIVER)
endif()

function(introsatlib_add_static_component target export_name)
	add_library("${target}" STATIC ${ARGN})
	add_library("IntroSatLib::${export_name}" ALIAS "${target}")
	set_target_properties("${target}" PROPERTIES EXPORT_NAME "${export_name}")
	target_compile_features("${target}" PUBLIC cxx_std_17)
	target_include_directories(
		"${target}"
		PUBLIC
			"$<BUILD_INTERFACE:${INTROSATLIB_SRC_DIR}>"
			"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/IntroSatLib>"
	)
	if(INTROSATLIB_ENABLE_INSTALL)
		install(
			TARGETS "${target}"
			EXPORT IntroSatLibTargets
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/IntroSatLib"
		)
	endif()
endfunction()

function(introsatlib_add_interface_component target export_name)
	add_library("${target}" INTERFACE)
	add_library("IntroSatLib::${export_name}" ALIAS "${target}")
	set_target_properties("${target}" PROPERTIES EXPORT_NAME "${export_name}")
	target_compile_features("${target}" INTERFACE cxx_std_17)
	target_include_directories(
		"${target}"
		INTERFACE
			"$<BUILD_INTERFACE:${INTROSATLIB_SRC_DIR}>"
			"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/IntroSatLib>"
	)
	if(INTROSATLIB_ENABLE_INSTALL)
		install(TARGETS "${target}" EXPORT IntroSatLibTargets)
	endif()
endfunction()

introsatlib_add_static_component(
	IntroSatLib_Core
	Core
	IS_Bluetooth.cpp
	ISL_Bootloader.cpp
	Logger.cpp
	${INTROSATLIB_ADAPTER_SOURCES}
)
target_compile_definitions(
	IntroSatLib_Core
	PUBLIC "${INTROSATLIB_BACKEND_DEFINITION}"
)
target_link_libraries(IntroSatLib_Core PUBLIC IntroSatLib::Platform)

add_subdirectory(Device)

introsatlib_add_static_component(
	IntroSatLib_Facade
	Facade
	Accelerometer.cpp
	AK8963.cpp
	Gyroscope.cpp
	LightSensor.cpp
)
target_link_libraries(IntroSatLib_Facade PUBLIC IntroSatLib::Device)

introsatlib_add_interface_component(IntroSatLib IntroSatLib)
target_link_libraries(
	IntroSatLib
	INTERFACE
		IntroSatLib::Facade
		${INTROSATLIB_DEVICE_TARGETS}
		IntroSatLib::Device
		IntroSatLib::Core
)

if(INTROSATLIB_ENABLE_INSTALL)
	install(
		DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/IntroSatLib"
		FILES_MATCHING PATTERN "*.h"
	)
endif()
