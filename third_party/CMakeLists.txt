cmake_minimum_required(VERSION 3.14)

include(FetchContent)
include(custom_fetch)

# libyuv

Custom_Fetch(
    libyuv
    https://github.com/lemenkov/libyuv.git
    main
)


# gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GMOCK "Install GMock" OFF)
option(INSTALL_GTEST "Install GTest" OFF)

FetchContent_MakeAvailable(googletest)

