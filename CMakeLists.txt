cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

set(CMAKE_C_STANDARD 90)

set(ZIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zig/src/main.zig)
set(ZIG_LIB ${CMAKE_BINARY_DIR}/libmain.so)

add_custom_command(
        OUTPUT ${ZIG_LIB} ${CMAKE_BINARY_DIR}/main.h
        COMMAND zig build-lib ${ZIG_SRC}
        -dynamic
        -O ReleaseSmall
        -femit-h
        DEPENDS ${ZIG_SRC}
        COMMENT "Building Zig library"
        VERBATIM
)

add_custom_target(ZigTarget ALL
        DEPENDS ${ZIG_LIB}
)

add_library(ZigLib SHARED IMPORTED)
set_target_properties(ZigLib PROPERTIES
        IMPORTED_LOCATION ${ZIG_LIB}
)

add_executable(WikiAnalyser
        src/main.c
        src/core/articleParser.c
        src/core/article.c
        src/core/cleanup.c
        src/core/unwanted.c
        src/GUI.c

        include/core/articleParser.h
        include/core/article.h
        include/core/cleanup.h
        include/core/unwanted.h
        include/GUI.h
        include/LINUXGUI.h
        src/LINUXGUI.c
)


add_dependencies(WikiAnalyser ZigTarget)

target_include_directories(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(WikiAnalyser PRIVATE ZigLib X11)
