set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/sample)
# add_executable(sample sample.cpp)

if (ENABLE_CXX11)
    add_executable(example example.cpp)
endif (ENABLE_CXX11)
