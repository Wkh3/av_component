set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(video_component ${sources})
target_link_libraries(video_component PRIVATE base_component libyuv)





