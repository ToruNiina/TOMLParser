#define BOOST_TEST_MODULE "test_utility"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/toml_parser.hpp"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_line_number)
{
    std::string contents("hoge\n fuga\n piyo");
    std::istringstream iss(contents);
    BOOST_CHECK_EQUAL(toml::line_number(iss), 1);

    std::string hoge;
    iss >> hoge;
    const std::istringstream::pos_type after_hoge = iss.tellg();
    BOOST_CHECK_EQUAL(toml::line_number(iss), 1);
    BOOST_CHECK_EQUAL(iss.tellg(), after_hoge);

    std::string fuga;
    iss >>fuga;
    const std::istringstream::pos_type after_fuga = iss.tellg();
    BOOST_CHECK_EQUAL(toml::line_number(iss), 2);
    BOOST_CHECK_EQUAL(iss.tellg(), after_fuga);
}

BOOST_AUTO_TEST_CASE(test_is_closed)
{
    const std::string closed_array_str("[1, 2, 3]");
    const bool closed_array = toml::is_closed(closed_array_str, '[', ']');
    BOOST_CHECK(closed_array);

    const std::string nested_array_str("[[1, 2], [3,4,5], [6]]");
    const bool nested_array = toml::is_closed(nested_array_str, '[', ']');
    BOOST_CHECK(nested_array);

    const std::string broken_array_str("[[1, 2]");
    const bool broken_array = toml::is_closed(broken_array_str, '[', ']');
    BOOST_CHECK(not broken_array);
}
