add_library(jsc context.cpp object.cpp value.cpp)

find_library(JAVASCRIPT_CORE JavaScriptCore)
target_link_libraries(jsc ${JAVASCRIPT_CORE})