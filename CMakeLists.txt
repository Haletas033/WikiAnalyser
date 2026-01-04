cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

set(CMAKE_C_STANDARD 90)

# Zig source
set(ZIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zig/src/main.zig)

# Custom target to build Zig as shared library
add_custom_target(ZigTarget ALL
        COMMAND zig build-lib ${ZIG_SRC} -dynamic -O ReleaseSmall -femit-h
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building Zig"
)

# Add the Zig library built by ZigTarget
add_library(Zig SHARED IMPORTED GLOBAL)
IF(WIN32)
    set_target_properties(Zig PROPERTIES
            IMPORTED_LOCATION_DEBUG ${CMAKE_BINARY_DIR}/main.dll
            IMPORTED_IMPLIB_DEBUG ${CMAKE_BINARY_DIR}/main.lib
    )
ELSE()
    set_target_properties(Zig PROPERTIES
            IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libmain.so
            INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}
    )
ENDIF()

# Your main C executable
add_executable(WikiAnalyser
        src/main.c
        src/parser.c
        src/article.c
        src/cleanup.c
        src/unwanted.c
        include/parser.h
        include/article.h
        include/cleanup.h
        include/unwanted.h
)

# Make sure C executable depends on Zig build
add_dependencies(WikiAnalyser ZigTarget)

# Link the Zig library
target_link_libraries(WikiAnalyser PRIVATE Zig)
