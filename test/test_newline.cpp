#define BOOST_TEST_MODULE "test_newline"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/toml_parser.hpp"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_is_newline)
{
    const std::string str0("");
    const std::string str1("\n");
    const std::string str2("\r\n");
    const std::string str3("hoge");
    const std::string str4("hoge\n");
    const std::string str5("hoge\r\n");

    std::istringstream iss0(str0);
    std::istringstream iss1(str1);
    std::istringstream iss2(str2);
    std::istringstream iss3(str3);
    std::istringstream iss4(str4);
    std::istringstream iss5(str5);

    BOOST_CHECK(not toml::is_newline(iss0));
    BOOST_CHECK(toml::is_newline(iss1));
    BOOST_CHECK(toml::is_newline(iss2));
    BOOST_CHECK(not toml::is_newline(iss3));
    BOOST_CHECK(not toml::is_newline(iss4));
    BOOST_CHECK(not toml::is_newline(iss5));

    for(std::size_t i=0; i<4; ++i)
    {
        iss3.ignore();
        iss4.ignore();
        iss5.ignore();
    }

    BOOST_CHECK(not toml::is_newline(iss3));
    BOOST_CHECK(toml::is_newline(iss4));
    BOOST_CHECK(toml::is_newline(iss5));
}
