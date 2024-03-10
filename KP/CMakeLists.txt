cmake_minimum_required(VERSION 3.22)
project(kp)

set(CMAKE_CXX_STANDARD 17)

add_library(sanitizers INTERFACE)
target_compile_options(sanitizers INTERFACE -fsanitize=address,undefined,integer)
target_link_options(sanitizers INTERFACE -fsanitize=address,undefined,integer)

add_executable(kp main.cpp)
target_link_libraries(kp sanitizers)

add_executable(client client.cpp)
target_link_libraries(client sanitizers)
