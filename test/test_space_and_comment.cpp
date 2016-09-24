#define BOOST_TEST_MODULE "test_space_and_comment"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/toml_parser.hpp"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_is_whitespace)
{
    const char space = ' ';
    const char tab = '\t';

    BOOST_CHECK(toml::is_whitespace(space));
    BOOST_CHECK(toml::is_whitespace(tab));

    for(char i = '!'; i <= '~'; ++i)
    {
        BOOST_CHECK(not toml::is_whitespace(i));
    }
}

BOOST_AUTO_TEST_CASE(test_skip_whitespace)
{
    const std::string str1("         indents");
    const std::string str2("extraneous      ");

    std::istringstream iss1(str1);
    std::istringstream iss2(str2);

    toml::skip_whitespace(iss1);
    toml::skip_whitespace(iss2);

    std::string result1;
    std::string result2;

    iss1 >> result1;
    iss2 >> result2;

    BOOST_CHECK_EQUAL(result1, std::string("indents"));
    BOOST_CHECK_EQUAL(result2, std::string("extraneous"));
}

BOOST_AUTO_TEST_CASE(test_comment_starts)
{
    BOOST_CHECK(not toml::comment_starts(' '));
    BOOST_CHECK(not toml::comment_starts('!'));
    BOOST_CHECK(not toml::comment_starts('"'));

    BOOST_CHECK(toml::comment_starts('#'));
    for(char i = '$'; i <= '~'; ++i)
    {
        BOOST_CHECK(not toml::comment_starts(i));
    }  
}

BOOST_AUTO_TEST_CASE(test_skip_comment)
{
    const std::string str1("# this is comment line\n");
    const std::string str2("# this is comment line\r\n");
    const std::string str3("# this is comment");
    const std::string str4("value # this is comment");

    std::istringstream iss1(str1);
    std::istringstream iss2(str2);
    std::istringstream iss3(str3);
    std::istringstream iss4(str4);

    toml::skip_comment(iss1);
    toml::skip_comment(iss2);
    toml::skip_comment(iss3);
    try
    {
        toml::skip_comment(iss4);
    }
    catch(toml::internal_error& except)
    {
        BOOST_CHECK_EQUAL(std::string(except.what()),
                          std::string("invalid skip_comment call"));
    }

    std::string result4;
    iss4 >> result4;

    BOOST_CHECK_EQUAL(iss1.peek(), std::char_traits<char>::eof());
    BOOST_CHECK_EQUAL(iss2.peek(), std::char_traits<char>::eof());
    BOOST_CHECK(iss3.eof());
    BOOST_CHECK_EQUAL(result4, std::string("value"));

    iss4 >> std::ws;
    toml::skip_comment(iss4);
    BOOST_CHECK(iss4.eof());
}
