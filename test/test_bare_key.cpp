#define BOOST_TEST_MODULE "test_bare_key"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/toml_parser.hpp"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_is_bare_key_component)
{
    for(char i = 'a'; i <= 'z'; ++i)
        BOOST_CHECK(toml::is_bare_key_component(i));

    for(char i = 'A'; i <= 'Z'; ++i)
        BOOST_CHECK(toml::is_bare_key_component(i));

    for(char i = '0'; i <= '9'; ++i)
        BOOST_CHECK(toml::is_bare_key_component(i));

    BOOST_CHECK(toml::is_bare_key_component('_'));
    BOOST_CHECK(toml::is_bare_key_component('-'));

    for(char i = ' '; i < '0'; ++i)
    {
        if(i == '-') continue;
        BOOST_CHECK(not toml::is_bare_key_component(i));
    }

    for(char i = ':'; i < 'A'; ++i)
    {
        BOOST_CHECK(not toml::is_bare_key_component(i));
    }
    for(char i = '['; i < 'a'; ++i)
    {
        if(i == '_') continue;
        BOOST_CHECK(not toml::is_bare_key_component(i));
    }
}

BOOST_AUTO_TEST_CASE(test_read_bare_key)
{
    const std::string key1_("key1 = value");
    std::istringstream key1(key1_);
    const std::string key1_r = toml::read_bare_key(key1);
    BOOST_CHECK_EQUAL(key1_r, std::string("key1"));

    const std::string key2_("key1-key2 = value");
    std::istringstream key2(key2_);
    const std::string key2_r = toml::read_bare_key(key2);
    BOOST_CHECK_EQUAL(key2_r, std::string("key1-key2"));

    const std::string key3_("key1_key2 = value");
    std::istringstream key3(key3_);
    const std::string key3_r = toml::read_bare_key(key3);
    BOOST_CHECK_EQUAL(key3_r, std::string("key1_key2"));

    const std::string key4_("key1_key2-key3 = value");
    std::istringstream key4(key4_);
    const std::string key4_r = toml::read_bare_key(key4);
    BOOST_CHECK_EQUAL(key4_r, std::string("key1_key2-key3"));

    const std::string key5_(" = value");
    std::istringstream key5(key5_);
    const std::string key5_r = toml::read_bare_key(key5);
    BOOST_CHECK_EQUAL(key5_r, std::string(""));

    const std::string key6_("= value");
    std::istringstream key6(key6_);
    const std::string key6_r = toml::read_bare_key(key6);
    BOOST_CHECK_EQUAL(key6_r, std::string(""));
}
