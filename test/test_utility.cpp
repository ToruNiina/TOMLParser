#define BOOST_TEST_MODULE "test_utility"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/utility.hpp"
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

BOOST_AUTO_TEST_CASE(test_read_integer)
{
    {
        std::string str("100words");
        std::istringstream iss(str);
        int num = toml::read_integer(iss, 3);
        BOOST_CHECK_EQUAL(num, 100);
    }

    {
        std::string str("1234words");
        std::istringstream iss(str);
        int num = toml::read_integer(iss, 3);
        BOOST_CHECK_EQUAL(num, 123);
    }

    {
        std::string str("12");
        std::istringstream iss(str);
        int num = toml::read_integer(iss, 3);
        BOOST_CHECK_EQUAL(num, 12);
    }

    {
        std::string str("");
        std::istringstream iss(str);
        iss.peek();
        int num = toml::read_integer(iss, 3);
        BOOST_CHECK_EQUAL(num, 0);
    }
}

BOOST_AUTO_TEST_CASE(test_get_numbers)
{
    {
        std::string str("100words");
        std::istringstream iss(str);
        std::string num = toml::get_numbers(iss);
        BOOST_CHECK_EQUAL(num, "100");
    }

    {
        std::string str("1234words");
        std::istringstream iss(str);
        std::string num = toml::get_numbers(iss);
        BOOST_CHECK_EQUAL(num, "1234");
    }

    {
        std::string str("12");
        std::istringstream iss(str);
        std::string num = toml::get_numbers(iss);
        BOOST_CHECK_EQUAL(num, "12");
    }

    {
        std::string str("");
        std::istringstream iss(str);
        std::string num = toml::get_numbers(iss);
        BOOST_CHECK_EQUAL(num, "");
    }
}
