add_library(${PROJECT_NAME}.jsc src/context.cpp src/object.cpp src/value.cpp)
set_target_properties(${PROJECT_NAME}.jsc PROPERTIES OUTPUT_NAME jscwrap)
target_compile_features(${PROJECT_NAME}.jsc PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}.jsc
                           INTERFACE include
                           PRIVATE include/jsc/details)

find_library(JAVASCRIPT_CORE JavaScriptCore)
target_link_libraries(${PROJECT_NAME}.jsc ${JAVASCRIPT_CORE})
