enable_testing()
include(GoogleTest)

macro(AddTests tests)

foreach(it ${tests})
        string(REGEX MATCH /[a-z,_]*.cpp$ des_str ${it})
        string(LENGTH ${des_str} str_len)
        math(EXPR len "${str_len} - 5")
        string(SUBSTRING ${des_str} 1 ${len} test)
        add_executable(${test} ${it})
        target_link_libraries(${test} PRIVATE gtest_main gmock)
        add_test(NAME ${test} COMMAND ${test})
endforeach()

endmacro()
