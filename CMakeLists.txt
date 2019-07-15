cmake_minimum_required(VERSION 3.14)
project(JSC-Test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(jsc)
add_subdirectory(jsast)

add_executable(jsc_test jsc_test.cpp)
target_link_libraries(jsc_test jsc)

add_executable(jsast_test jsast_test.cpp)
target_link_libraries(jsast_test jsast)
