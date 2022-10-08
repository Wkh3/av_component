cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# libyuv
FetchContent_Declare(
    libyuv
    GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv
    GIT_TAG        b179f1847a7cc17957eab399610cb9ef163bb715
)

FetchContent_MakeAvailable(libyuv)

# gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)


FetchContent_MakeAvailable(googletest)
