cmake_minimum_required(VERSION 3.16)
project(TaskManager LANGUAGES CXX)

add_executable(task_manager_tests
    main.cpp
    Person.cpp
    Task.cpp
    TaskManager.cpp
)

target_compile_features(task_manager_tests PRIVATE cxx_std_17)
set_target_properties(task_manager_tests PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(task_manager_tests PRIVATE /W4)
else()
    target_compile_options(task_manager_tests PRIVATE -Wall -Werror -pedantic-errors)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(regression_tests tests/regression.cpp Task.cpp Person.cpp TaskManager.cpp)
    target_include_directories(regression_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_features(regression_tests PRIVATE cxx_std_17)
    if(MSVC)
        target_compile_options(regression_tests PRIVATE /W4)
    else()
        target_compile_options(regression_tests PRIVATE -Wall -Werror -pedantic-errors)
    endif()
    add_test(NAME regression COMMAND regression_tests)
    add_test(NAME task_manager COMMAND task_manager_tests)
    set_tests_properties(task_manager PROPERTIES
        FAIL_REGULAR_EXPRESSION "Assertion failed;\\[Failed\\]"
    )
endif()
