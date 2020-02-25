cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(UNIXNetworkProgramming)
add_subdirectory(InsideTheC++ObjectModel)
add_subdirectory(APUE)
add_subdirectory(c++11_14_17新增特性)
add_subdirectory(网络通信基础重难点解析)
add_subdirectory(侯婕)
add_subdirectory(网络通信故障排查常用命令)
add_subdirectory(LinuxGDB调试指南介绍)
add_subdirectory(Redis网络通信框架源码解析)
add_subdirectory(网络通信协议设计专题)
add_subdirectory(高性能服务的基本结构)

add_executable(main main.cpp)

