cmake_minimum_required(VERSION 3.1)
project(mux-demux)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2")

set(SOURCE_FILES 
  src/simple.cpp
)

add_executable(simple src/simple.cpp)
target_include_directories(simple PRIVATE include)

add_executable(client_server src/client_server.cpp)
target_include_directories(client_server PRIVATE include)

