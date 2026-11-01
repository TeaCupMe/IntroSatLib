# Headless ISP-lite / TCP demo — does not require Cube/Arduino.
# Enabled from root with -DINTROSATLIB_BUILD_HEADLESS=ON
# or: cmake -S headless -B build-headless && cmake --build build-headless

cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	project(IntroSatLibHeadless LANGUAGES CXX)
endif()

add_library(
	introsatlib_headless_tcp
	STATIC
	tcp/TcpI2CBus.cpp
	tcp/TcpIspServer.cpp
)
target_compile_features(introsatlib_headless_tcp PUBLIC cxx_std_17)
target_include_directories(
	introsatlib_headless_tcp
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/isp"
		"${CMAKE_CURRENT_SOURCE_DIR}/tcp"
		"${CMAKE_CURRENT_SOURCE_DIR}/log"
		"${CMAKE_CURRENT_SOURCE_DIR}/../src"
)

add_executable(introsatlib_sim_lm75a sim_lm75a.cpp)
target_link_libraries(introsatlib_sim_lm75a PRIVATE introsatlib_headless_tcp)

add_executable(introsatlib_headless_demo demo_main.cpp)
target_link_libraries(introsatlib_headless_demo PRIVATE introsatlib_headless_tcp)

message(STATUS "IntroSatLib headless TCP/ISP-lite demos enabled")
