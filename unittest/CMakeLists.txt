cmake_minimum_required(VERSION 3.2)
project(Tests)

find_package(GTest CONFIG REQUIRED)
add_executable(Tests test_dot.cpp)

target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main)

# discover tests
include(GoogleTest)
gtest_discover_tests(Tests)