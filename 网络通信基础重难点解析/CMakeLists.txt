cmake_minimum_required(VERSION 3.13)
project(node)

set(CMAKE_CXX_STANDARD 14)


add_executable(select_server select_server.cpp)
add_executable(select_client select_client.cpp)
add_executable(blocking_server blocking_server.cpp)
add_executable(blocking_client blocking_client.cpp)
add_executable(server_recv_zero_bytes server_recv_zero_bytes.cpp)
add_executable(nonblocking_client_send_zero_bytes nonblocking_client_send_zero_bytes.cpp)