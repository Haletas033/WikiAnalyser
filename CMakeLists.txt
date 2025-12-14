cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser)

set(CMAKE_C_STANDARD 90)

add_executable(WikiAnalyser
        src/main.c
        src/parser.c
        src/article.c
        include/parser.h
        include/article.h
)