cmake_minimum_required(VERSION 3.20)

project(Typhon)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

add_executable(tyc
    source/main.cpp source/lexical_analysis.cpp)

