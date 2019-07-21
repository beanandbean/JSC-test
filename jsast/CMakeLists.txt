add_library(${PROJECT_NAME}.jsast src/generator.cpp)
set_target_properties(${PROJECT_NAME}.jsast PROPERTIES OUTPUT_NAME jsast)
target_compile_features(${PROJECT_NAME}.jsast PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}.jsast INTERFACE include)
target_include_directories(${PROJECT_NAME}.jsast PRIVATE include/jsast/details)
