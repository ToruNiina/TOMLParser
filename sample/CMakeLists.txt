set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/sample)

if (TOML_ENABLE_CXX11)
    add_executable(sample sample.cpp)
    add_executable(example example.cpp)
endif (TOML_ENABLE_CXX11)
