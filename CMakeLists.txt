cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(SUBPROJECT ON)
endif()

project(jscutils LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(jsc)
add_subdirectory(jsast)

if(NOT SUBPROJECT)
  add_executable(jsast_test jsast_test.cpp)
  set_target_properties(jsast_test PROPERTIES OUTPUT_NAME jsast.out)
  target_link_libraries(jsast_test ${PROJECT_NAME}.jsast)

  add_executable(jsc_test jsc_test.cpp)
  set_target_properties(jsc_test PROPERTIES OUTPUT_NAME jsc.out)
  target_link_libraries(jsc_test ${PROJECT_NAME}.jsc)
endif()
