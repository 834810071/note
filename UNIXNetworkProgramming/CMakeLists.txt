cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 14)

add_executable(daytimetcpcli daytimetcpcli.cpp)
add_executable(daytimetcpsrv daytimetcpsrv.cpp)
add_executable(byteorder byteorder.cpp)
add_executable(tcpcli01 tcpcli01.cpp)
add_executable(tcpserv01 tcpserv01.cpp)
add_executable(tcpservselect01 tcpservselect01.cpp)
add_executable(tcpservpoll01 tcpservpoll01.cpp)
add_executable(hostent hostent.cpp)