# The subdirectories to build
set(SUBDIRS bluetooth)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

source_group("Header Files" FILES ${HEADERS})
add_library(Connectors ${SOURCE} ${HEADERS})
target_link_libraries(Connectors BluetoothConnector)