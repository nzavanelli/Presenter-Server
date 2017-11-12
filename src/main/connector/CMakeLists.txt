# The subdirectories to build
set(SUBDIRS bluetooth)

# Build all files in this directory
SET(SOURCE
    RemoteControl.cpp
    key_sender.c
)

SET(HEADERS
    RemoteControl.h
    key_sender.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(RemoteControl ${SOURCE} ${HEADERS})
target_link_libraries(RemoteControl Qt5::Core)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

# This "library" is used to allow access to all installed connectors
add_library(Connectors)
target_link_libraries(Connectors BluetoothConnector)