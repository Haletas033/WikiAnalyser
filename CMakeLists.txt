cmake_minimum_required(VERSION 3.31)
project(WikiAnalyser)

set(CMAKE_CXX_STANDARD 98)

add_executable(WikiAnalyser main.cpp
        include/parser.h)
