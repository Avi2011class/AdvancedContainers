cmake_minimum_required(VERSION 3.5)

project(decartian)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -std=c++17 -march=native")

include_directories(./)

add_executable(Decartian tests.cpp decartian.hpp nodes.hpp)

target_link_libraries(Decartian gtest gtest_main pthread)

