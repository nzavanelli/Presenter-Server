# Project name and version
project(Presenter_Server)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_BETA 1)

# The subdirectories to build
set(SUBDIRS main test)

# Minimum cmake version
cmake_minimum_required(VERSION 3.5.1)

# Enable testing
enable_testing()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Set compiler flags on g++
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    # Here the -D_DEBUG is needed because it's not set under cmake in ubuntu
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -ggdb -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Set some Win Specific Settings
if(WIN32)
    set(GUI_TYPE WIN32) # Disables console

    if(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP")
        include_directories("C:/Program Files (x86)/Visual Leak Detector/include")
        if (CMAKE_CL_64)
            link_directories("C:/Program Files (x86)/Visual Leak Detector/lib/Win64")
        else(CMAKE_CL_64)
            link_directories("C:/Program Files (x86)/Visual Leak Detector/lib/Win32")
        endif(CMAKE_CL_64)
    endif(MSVC)
else(WIN32)
    set(GUI_TYPE )
    set(RSRC )
endif(WIN32)

if(CMAKE_CXX_FLAGS_RELEASE)
    add_definitions(-DUSED_CXXFLAGS="${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_CXX_FLAGS_RELEASE)
if(CMAKE_CXX_FLAGS_DEBUG)
    add_definitions(-DUSED_CXXFLAGS_DEBUG="${CMAKE_CXX_FLAGS_DEBUG}")
endif(CMAKE_CXX_FLAGS_DEBUG)

# Enable code coverage on linux
if(UNIX)
    add_custom_target (coverage
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/coverage.sh
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(UNIX)

# Set some Apple MacOS Specific settings
if(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
endif(APPLE)

# Linux specific settings. We need a key sender daemon running as priviledged
# user to inject events
if(UNIX)
    # The network port to use for linux daemon.
    # Stay above 1024 to allow usage without root if the user configured
    # the access to the input device without root rights.
    set(KEYSENDER_PORT 1111)

    # Generate the port config header
    configure_file("daemon_port.h.in" "${PROJECT_BINARY_DIR}/daemon_port.h")

    # Build the daemon
    set(SUBDIRS ${SUBDIRS} keysenderDaemon)
endif(UNIX)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)
