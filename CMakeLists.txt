cmake_minimum_required(VERSION 3.3)
project(f4v_parseer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp f4v_type.hpp f4v_log.hpp f4v_log.cpp f4v_parse.cpp f4v_parse.hpp f4v_utility.cpp f4v_utility.hpp f4v_box.cpp f4v_box.hpp)
add_executable(f4v_parseer ${SOURCE_FILES})