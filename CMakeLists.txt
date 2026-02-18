cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

#Terminate if on MacOS
if (APPLE)
    message(FATAL_ERROR "MacOS is currently not supported")
endif()

set(CMAKE_C_STANDARD 90)

set(ZIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zig/src/main.zig)

if (WIN32)
    set(ZIG_DLL ${CMAKE_BINARY_DIR}/main.dll)
    set(ZIG_LIB ${CMAKE_BINARY_DIR}/main.lib)
else ()
    set(ZIG_DLL)
    set(ZIG_LIB ${CMAKE_BINARY_DIR}/libmain.so)
endif()

add_custom_command(
        OUTPUT ${ZIG_LIB} ${ZIG_DLL} ${CMAKE_BINARY_DIR}/main.h
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

if (WIN32)
    set_target_properties(ZigLib PROPERTIES
            IMPORTED_LOCATION ${ZIG_DLL}
            IMPORTED_IMPLIB ${ZIG_LIB}
    )
else ()
    set_target_properties(ZigLib PROPERTIES
            IMPORTED_LOCATION ${ZIG_LIB}
    )
endif()

if (WIN32)
    set(OS_SRC
            src/WINGUI.c
    )
    set(OS_INCLUDE
            include/WINGUI.h
    )
else()
    set(OS_SRC
            src/LINUXGUI.c
    )
    set(OS_INCLUDE
            include/LINUXGUI.h
    )
endif()

add_executable(WikiAnalyser
        ${OS_SRC}
        ${OS_INCLUDE}
        src/main.c
        src/core/articleParser.c
        src/core/article.c
        src/core/cleanup.c
        src/core/unwanted.c
        src/GUI.c
        src/welcomeGUI.c

        include/core/articleParser.h
        include/core/article.h
        include/core/cleanup.h
        include/core/unwanted.h
        include/GUI.h
        include/welcomeGUI.h


)

add_dependencies(WikiAnalyser ZigTarget)
target_include_directories(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR})

if (WIN32)
    target_link_libraries(WikiAnalyser PRIVATE ZigLib)
else ()
    target_link_libraries(WikiAnalyser PRIVATE ZigLib X11 m)
endif()

if (UNIX)

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
endif()