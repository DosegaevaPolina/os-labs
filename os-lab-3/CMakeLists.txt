cmake_minimum_required(VERSION 3.22)

project(lab3)

set(CMAKE_C_STANDARD 11)

add_executable(child1 to_lower.c)

add_executable(child2 unify_spaces.c)

set(REQUIRED_FILES child1 child2)

add_executable(main3 main.c)
