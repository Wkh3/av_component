set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE error_tests "${CMAKE_CURRENT_SOURCE_DIR}/error/*_test.cpp")

add_library(base_component ${sources})
target_include_directories(base_component PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


include(testing)

AddTests(${error_tests} base_component)