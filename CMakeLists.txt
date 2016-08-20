project(toml)
cmake_minimum_required(VERSION 2.8)
enable_testing()

add_definitions("-std=c++11")
include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(sample)
