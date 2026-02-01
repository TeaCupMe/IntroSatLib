cmake_minimum_required(VERSION 3.15)
project(mylib)

# Define the source files for the library
set(MYLIB_SOURCES
    src/Adapter/VIRTUAL/VIRTUAL_System.cpp
    src/Logger.cpp
    # src/anotherfile.cpp
    # Add all source files here
)

# Add the library target
# SHARED creates a dynamic library (.so, .dll, .dylib)
# STATIC creates a static library (.a, .lib)
add_library(IntroSatLib STATIC ${MYLIB_SOURCES})

# Specify include directories for the library itself
target_include_directories(IntroSatLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src 
    # Or simply a local 'include' folder
)

# (Optional) If the library will be installed for other projects to find
# include(GNUInstallDirs)
# install(TARGETS IntroSatLib
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )