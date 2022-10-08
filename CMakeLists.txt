cmake_minimum_required(VERSION 3.2)

project(av_component 
        VERSION 1.0.0
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(base)
add_subdirectory(video)
add_subdirectory(third_party)
