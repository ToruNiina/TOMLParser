project(toml)
cmake_minimum_required(VERSION 2.8)
enable_testing()

include_directories("${PROJECT_SOURCE_DIR}")

option(TOML_ENABLE_CXX11 "use c++11 standard" ON)
option(TOML_ENABLE_DEBUG "output information" OFF)

if(TOML_ENABLE_CXX11)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_definitions( -std=c++11 )
else(TOML_ENABLE_CXX11)
    message(WARNING "c++11 disabled. compile with c++98 and boost c++ library.")
    find_package(Boost REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})
endif(TOML_ENABLE_CXX11)

if(TOML_ENABLE_DEBUG)
    add_definitions(-DTOML_DEBUG)
endif(TOML_ENABLE_DEBUG)

add_subdirectory(test)
add_subdirectory(sample)
