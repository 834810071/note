cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(UNIXNetworkProgramming)

add_executable(main main.cpp)

