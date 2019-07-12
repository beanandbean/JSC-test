cmake_minimum_required(VERSION 3.14)
project(JSC-Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(jsc)

add_executable(test main.cpp)
target_link_libraries(test jsc)