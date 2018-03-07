# The subdirectories to build
set(SUBDIRS bluetooth)

# Build all files in this directory
SET(SOURCE
    RemoteControl.cpp
    KeySender.cpp
)

SET(HEADERS
    RemoteControl.h
    KeySender.h
)

# For windows we can directly include the key sender into our binary
if(WIN32)
    set(SOURCE ${SOURCE} key_sender.c)
    set(HEADERS ${HEADERS} key_sender.h)
endif(WIN32)

source_group("Header Files" FILES ${HEADERS})
add_library(RemoteControl ${SOURCE} ${HEADERS})
target_link_libraries(RemoteControl Qt5::Core)

# For linux, we connect to a daemon that will emit the keys
if(UNIX)
    # Build key sender as library so that it can be included into daemon
    add_library(key_sender key_sender.c key_sender.h)

    find_package(Qt5Network REQUIRED)
    target_link_libraries(RemoteControl Qt5::Network)
endif(UNIX)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

# This "library" is used to allow access to all installed connectors
add_library(Connectors)
target_link_libraries(Connectors BluetoothConnector)
