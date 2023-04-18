set_project("av_component")

add_requires("spdlog","abseil 20230125.2")
add_requires("gtest 1.12.0",{configs = {main = true}})

add_includedirs("./src")

target("base_component")
    set_kind("static")
    set_languages("c++17")
    add_files("src/base/error/*.cpp")     
    -- add_files("src/base/pipeline/*.cpp")     
    add_files("src/base/task_queue/*.cpp")
    add_files("src/base/executor/*.cpp")
    add_files("src/base/*.cpp")
    add_packages("spdlog","abseil")

target("test_main")
    set_kind("static")
    set_languages("c++17")
    add_files("src/base/test/*.cpp")
    add_packages("gtest")

target("test")
    set_kind("binary")
    set_languages("c++17")
    set_group("tests")
    add_files("src/base/error/test/*_test.cpp")
    add_files("src/base/executor/test/*_test.cpp")
    add_files("src/base/test/main.cpp")
    add_deps("base_component")
    add_packages("gtest","abseil")