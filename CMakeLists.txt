cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser C)

set(CMAKE_C_STANDARD 90)

set(ZIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zig/src/main.zig)
set(ZIG_DLL ${CMAKE_BINARY_DIR}/main.dll)
set(ZIG_LIB ${CMAKE_BINARY_DIR}/main.lib)

add_custom_target(ZigTarget ALL
        COMMAND zig build-lib ${ZIG_SRC} -dynamic -O ReleaseSmall -femit-h
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building Zig"
)

add_library(Zig SHARED IMPORTED GLOBAL)
set_target_properties(Zig PROPERTIES
        IMPORTED_IMPLIB ${ZIG_LIB}
        IMPORTED_LOCATION ${ZIG_DLL}
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}
)

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

add_dependencies(WikiAnalyser ZigTarget)

target_link_libraries(WikiAnalyser PRIVATE Zig)

add_custom_command(TARGET WikiAnalyser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/main.dll
        $<TARGET_FILE_DIR:WikiAnalyser>
)
