cmake_minimum_required(VERSION 3.16)
project(ducky C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
