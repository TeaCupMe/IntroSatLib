cmake_minimum_required(VERSION 3.15)
project(IntroSatLib)


add_library(introsatlib STATIC
    src/Adapter/VIRTUAL/VIRTUAL_System.cpp
    src/Adapter/VIRTUAL/VIRTUAL_I2C.cpp
    src/Device/I2CDevice.cpp
    src/Device/LSM6DS3/LSM6DS3.cpp
    src/Device/LIS3MDL/LIS3MDL.cpp
    src/Logger.cpp
)

target_include_directories(introsatlib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src 
)