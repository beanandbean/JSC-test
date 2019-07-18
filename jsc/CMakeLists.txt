add_library(jsc src/context.cpp src/object.cpp src/value.cpp)
target_compile_features(jsc PUBLIC cxx_std_17)

target_include_directories(jsc INTERFACE include)
target_include_directories(jsc PRIVATE include/jsc/details)

find_library(JAVASCRIPT_CORE JavaScriptCore)
target_link_libraries(jsc ${JAVASCRIPT_CORE})
