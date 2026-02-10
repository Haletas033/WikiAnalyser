cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

set(CMAKE_C_STANDARD 90)

#Zig source
set(ZIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zig/src/main.zig)
set(ZIG_EXE "zig")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

#Custom command to generate the DLL/lib/header
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/main.dll ${CMAKE_BINARY_DIR}/main.lib ${CMAKE_BINARY_DIR}/main.h
        COMMAND ${ZIG_EXE} build-lib ${ZIG_SRC} -dynamic -O ReleaseSmall -femit-h
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building Zig library"
        VERBATIM
)

#Custom target that depends on the above outputs
add_custom_target(ZigTarget ALL
        DEPENDS ${CMAKE_BINARY_DIR}/main.dll ${CMAKE_BINARY_DIR}/main.lib ${CMAKE_BINARY_DIR}/main.h
)

add_executable(WikiAnalyser
        src/main.c
        src/core/articleParser.c
        src/core/article.c
        src/core/cleanup.c
        src/core/unwanted.c
        include/core/articleParser.h
        include/core/article.h
        include/core/cleanup.h
        include/core/unwanted.h
)

add_dependencies(WikiAnalyser ZigTarget)

target_include_directories(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(WikiAnalyser PRIVATE ${CMAKE_BINARY_DIR}/main.lib)
