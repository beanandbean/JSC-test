cmake_minimum_required(VERSION 3.14)
project(JSC-Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(jsc)
add_subdirectory(jscodegen)

add_executable(jsc_test jsc_test.cpp)
target_link_libraries(jsc_test jsc)

add_executable(jscodegen_test jscodegen_test.cpp)
# target_link_libraries(jscodegen_test jscodegen)
