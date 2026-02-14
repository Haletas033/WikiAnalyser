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
        src/LINUXGUI.c
        src/WINGUI.c
        include/core/articleParser.h
        include/core/article.h
        include/core/cleanup.h
        include/core/unwanted.h
        include/GUI.h
        include/LINUXGUI.h
        include/WINGUI.h
)

add_dependencies(WikiAnalyser ZigTarget)
target_include_directories(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(WikiAnalyser PRIVATE ZigLib X11 m)

find_package(PkgConfig REQUIRED)
pkg_check_modules(XFT REQUIRED xft)
pkg_check_modules(FREETYPE REQUIRED freetype2)

target_include_directories(WikiAnalyser PRIVATE
        ${XFT_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
)
target_link_libraries(WikiAnalyser PRIVATE
        ${XFT_LIBRARIES}
        ${FREETYPE_LIBRARIES}
)
target_compile_options(WikiAnalyser PRIVATE
        ${XFT_CFLAGS_OTHER}
        ${FREETYPE_CFLAGS_OTHER}
)
