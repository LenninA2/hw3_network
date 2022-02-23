cmake_minimum_required(VERSION 3.1)
project(checksum)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2")

add_executable(checksum src/checksum.c)
add_executable(checksum_fail src/checksum_fail.c)
