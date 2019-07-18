add_library(jsast src/generator.cpp)
target_compile_features(jsast PUBLIC cxx_std_17)

target_include_directories(jsast INTERFACE include)
target_include_directories(jsast PRIVATE include/jsast/details)
