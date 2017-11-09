# Build all files in this directory
SET(SOURCE
    BluetoothConnectorBase.cpp
    key_sender.c
)

SET(HEADERS
    BluetoothConnectorBase.h
    BluetoothConnector.h
    key_sender.h
)

if (WIN32)
    set(SOURCE ${SOURCE} BluetoothConnector_Windows.cpp)
    set(HEADERS ${HEADERS} BluetoothConnector_Windows.h)
endif(WIN32)

if (UNIX)
    set(SOURCE ${SOURCE} BluetoothConnector_Linux.cpp)
    set(HEADERS ${HEADERS} BluetoothConnector_Linux.h)
    
    find_package(Qt5Bluetooth REQUIRED)
endif(UNIX)

source_group("Header Files" FILES ${HEADERS})

add_library(BluetoothConnector ${SOURCE} ${HEADERS})

if (WIN32)
    target_link_libraries(BluetoothConnector Qt5::Core wsock32 ws2_32)
endif(WIN32)
if (UNIX)
    target_link_libraries(BluetoothConnector Qt5::Bluetooth)
endif(UNIX)