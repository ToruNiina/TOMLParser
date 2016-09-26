set(TEST_NAMES
    test_newline
    test_space_and_comment
    test_utility
    test_bare_key
    test_read_value
    test_parse_value
    )

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
include_directories("${PROJECT_SOURCE_DIR}/src")

set(test_library_dependencies)
find_library(BOOST_UNITTEST_FRAMEWORK_LIBRARY boost_unit_test_framework)
if (BOOST_UNITTEST_FRAMEWORK_LIBRARY)
    add_definitions(-DBOOST_TEST_DYN_LINK)
    add_definitions(-DUNITTEST_FRAMEWORK_LIBRARY_EXIST)
    set(test_library_dependencies boost_unit_test_framework)
endif()

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} ${test_library_dependencies})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_NAME)

add_executable(test_file_reading_test test_file_reading_test.cpp)
target_link_libraries(test_file_reading_test ${test_library_dependencies})
add_test(NAME test_file_reading_test COMMAND test_file_reading_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

