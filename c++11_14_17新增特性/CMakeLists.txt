cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_01 test01.cpp)
target_link_libraries(test_01 pthread)