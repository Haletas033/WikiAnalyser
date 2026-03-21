cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

#Terminate if on MacOS
if (APPLE)
    message(FATAL_ERROR "MacOS is currently not supported")
endif()

set(CMAKE_C_STANDARD 90)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

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
            include/config.h
            src/curl.c
            src/topN.c

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
        src/mainGUI.c

        include/core/articleParser.h
        include/core/article.h
        include/core/cleanup.h
        include/core/unwanted.h
        include/GUI.h
        include/welcomeGUI.h
        include/mainGUI.h
        src/config.c
        include/curl.h
        src/curl.c
        include/topN.h
        src/topN.c
)

target_include_directories(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR})

if (WIN32)
    target_link_libraries(WikiAnalyser PRIVATE -lshlwapi)
else ()
    target_link_libraries(WikiAnalyser PRIVATE X11 m)
endif()

if (UNIX)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XFT REQUIRED xft)
    pkg_check_modules(FREETYPE REQUIRED freetype2)

    target_include_directories(WikiAnalyser PRIVATE
            ${XFT_INCLUDE_DIRS}
            ${FREETYPE_INCLUDE_DIRS}
    )
    target_compile_options(WikiAnalyser PRIVATE
            ${XFT_CFLAGS_OTHER}
            ${FREETYPE_CFLAGS_OTHER}
    )
endif()

target_link_libraries(WikiAnalyser PRIVATE
        ${XFT_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        CURL::libcurl
        ZLIB::ZLIB
)