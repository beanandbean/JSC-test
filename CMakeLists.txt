cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(SUBPROJECT ON)
endif()

project(jsc-wrapper LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(jsc)
add_subdirectory(jsast)

if(NOT SUBPROJECT)
  add_executable(__jsast_test__ jsast_test.cpp)
  set_target_properties(__jsast_test__ PROPERTIES OUTPUT_NAME jsast.out)
  target_link_libraries(__jsast_test__ jsast)

  add_executable(__jsc_test__ jsc_test.cpp)
  set_target_properties(__jsc_test__ PROPERTIES OUTPUT_NAME jsc.out)
  target_link_libraries(__jsc_test__ jsc)
endif()
