cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 14)

add_executable(myls myls.cpp)
add_executable(out out.cpp)
add_executable(a a.cpp)
add_executable(file file.cpp)

target_link_libraries(myls apue)