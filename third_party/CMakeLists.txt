cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# libyuv
FetchContent_Declare(
    libyuv
    GIT_REPOSITORY https://github.com/lemenkov/libyuv.git
    GIT_TAG main
)

FetchContent_MakeAvailable(libyuv)

# gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)

include(CMakePrintHelpers)
cmake_print_properties(TARGETS gtest
                        PROPERTIES TYPE SOURCE_DIR)
cmake_print_properties(TARGETS yuv_shared
                        PROPERTIES TYPE SOURCE_DIR)

