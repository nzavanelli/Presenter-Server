# Build the key sender as daemon so that it can be executed as root and pass through the key events
if(UNIX)
    find_package(Qt5Network REQUIRED)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../main/connector)
    link_directories(${CMAKE_CURRENT_BINARY_DIR}/../main/connector)

    add_executable(${CMAKE_PROJECT_NAME}_Keysender_Daemon KeySenderDaemonMain.cpp
        KeySenderDaemon.cpp KeySenderDaemon.h)
    target_link_libraries(${CMAKE_PROJECT_NAME}_Keysender_Daemon key_sender Qt5::Core Qt5::Network)
endif(UNIX)
