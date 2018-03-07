# The subdirectories to build
set(SUBDIRS gui connector)

# Generate the version file
configure_file(
    "Version.h.in"
    "${PROJECT_BINARY_DIR}/Version.h"
)

# Make version file readable for project
include_directories("${PROJECT_BINARY_DIR}")

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

# Image resource files
set(CMAKE_AUTORCC ON)
set(RSRC res/images.qrc)
if(WIN32)
    set(RSRC "${RSRC}" res/images/msw/msw.rc)
endif(WIN32)

# Generate the main program
source_group("Header Files" FILES Version.h)
add_executable(${CMAKE_PROJECT_NAME} ${GUI_TYPE} ${RSRC} Main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} Gui)

if(UNIX)
    # Publish the runner script
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/main/res/scripts/presenter_server.sh
                ${CMAKE_BINARY_DIR}/presenter_server.sh)
endif(UNIX)
