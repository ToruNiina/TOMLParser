project(toml)
cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_CXX_COMPILER clang++-3.6)
add_definitions("-std=c++11")
include_directories("${CMAKE_SOURCE_DIR}/src")

add_subdirectory(sample)
