add_requires("gtest","spdlog","abseil 20230125.2")

add_includedirs("./src")
target("base_component")
    -- set_kind("static")
    set_kind("static")
    set_languages("c++17")
    add_files("src/base/error/*.cpp")     
    -- add_files("src/base/pipeline/*.cpp")     
    add_files("src/base/task_queue/*.cpp")
    add_files("src/base/*.cpp")
    add_packages("gtest","spdlog","abseil")