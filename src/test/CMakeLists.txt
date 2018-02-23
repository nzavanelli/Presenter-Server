find_package(Qt5Test REQUIRED)

# The subdirectories to build
set(SUBDIRS gui)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SUB})
    link_directories(${CMAKE_CURRENT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)