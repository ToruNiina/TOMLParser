#define BOOST_TEST_MODULE "test_read_value"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "src/toml_parser.hpp"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_read_array)
{
    const std::string simple_array_str("[1,2,3]");
    std::istringstream simple_array(simple_array_str);
    const std::string simple_array_result = toml::read_array(simple_array);
    BOOST_CHECK_EQUAL(simple_array_result, simple_array_str);

    const std::string simple_array_str2("[1,2,3], ");
    std::istringstream simple_array2(simple_array_str2);
    const std::string simple_array_result2 = toml::read_array(simple_array2);
    BOOST_CHECK_EQUAL(simple_array_result2, simple_array_str);

    const std::string nested_array_str("[[1,2,3],[4,5]]");
    std::istringstream nested_array(nested_array_str);
    const std::string nested_array_result = toml::read_array(nested_array);
    BOOST_CHECK_EQUAL(nested_array_result, nested_array_str);

    const std::string nested_array_str2("[[1,2,3],[4,5]],  hoge");
    std::istringstream nested_array2(nested_array_str2);
    const std::string nested_array_result2 = toml::read_array(nested_array2);
    BOOST_CHECK_EQUAL(nested_array_result2, nested_array_str);
}

BOOST_AUTO_TEST_CASE(test_read_inline_table)
{
    const std::string inline_table_str("{key = \"value\"}");
    std::istringstream inline_table(inline_table_str);
    const std::string inline_table_result = toml::read_inline_table(inline_table);
    BOOST_CHECK_EQUAL(inline_table_result, inline_table_str);

    const std::string inline_table_str_("{key = \"value\"}, hoge");
    std::istringstream inline_table_(inline_table_str_);
    const std::string inline_table_result_ = toml::read_inline_table(inline_table_);
    BOOST_CHECK_EQUAL(inline_table_result_, inline_table_str);

    const std::string nested_table_str("{key = {key = nested}}");
    std::istringstream nested_table(nested_table_str);
    const std::string nested_table_result = toml::read_inline_table(nested_table);
    BOOST_CHECK_EQUAL(nested_table_result, nested_table_str);

    const std::string nested_table_str_("{key = {key = nested}}, piyo");
    std::istringstream nested_table_(nested_table_str_);
    const std::string nested_table_result_ = toml::read_inline_table(nested_table_);
    BOOST_CHECK_EQUAL(nested_table_result_, nested_table_str);
}

BOOST_AUTO_TEST_CASE(test_read_string)
{
    // basic

    const std::string inline_basic_string_str("\"basic string.\"");
    std::istringstream inline_basic_string(inline_basic_string_str);
    const std::string inline_basic_string_result = 
        toml::read_string(inline_basic_string);
    BOOST_CHECK_EQUAL(inline_basic_string_result, inline_basic_string_str);

    const std::string inline_basic_string_str_("\"basic string.\" , hoge");
    std::istringstream inline_basic_string_(inline_basic_string_str_);
    const std::string inline_basic_string_result_ = 
        toml::read_string(inline_basic_string_);
    BOOST_CHECK_EQUAL(inline_basic_string_result_, inline_basic_string_str);

    const std::string multiline_basic_string_str(
            "\"\"\"multiline basic\n string\"\"\"");
    std::istringstream multiline_basic_string(multiline_basic_string_str);
    const std::string multiline_basic_string_result =
        toml::read_string(multiline_basic_string);
    BOOST_CHECK_EQUAL(multiline_basic_string_result, multiline_basic_string_str);

    const std::string multiline_basic_string_str_(
            "\"\"\"multiline basic\n string\"\"\", hoge");
    std::istringstream multiline_basic_string_(multiline_basic_string_str_);
    const std::string multiline_basic_string_result_ =
        toml::read_string(multiline_basic_string_);
    BOOST_CHECK_EQUAL(multiline_basic_string_result_, multiline_basic_string_str);

    // literal

    const std::string inline_literal_string_str("\'literal string.\'");
    std::istringstream inline_literal_string(inline_literal_string_str);
    const std::string inline_literal_string_result = 
        toml::read_string(inline_literal_string);
    BOOST_CHECK_EQUAL(inline_literal_string_result, inline_literal_string_str);

    const std::string inline_literal_string_str_("\'literal string.\' , hoge");
    std::istringstream inline_literal_string_(inline_literal_string_str_);
    const std::string inline_literal_string_result_ = 
        toml::read_string(inline_literal_string_);
    BOOST_CHECK_EQUAL(inline_literal_string_result_, inline_literal_string_str);

    const std::string multiline_literal_string_str(
            "\'\'\'multiline literal\n string\'\'\'");
    std::istringstream multiline_literal_string(multiline_literal_string_str);
    const std::string multiline_literal_string_result =
        toml::read_string(multiline_literal_string);
    BOOST_CHECK_EQUAL(multiline_literal_string_result, multiline_literal_string_str);

    const std::string multiline_literal_string_str_(
            "\'\'\'multiline literal\n string\'\'\', hoge");
    std::istringstream multiline_literal_string_(multiline_literal_string_str_);
    const std::string multiline_literal_string_result_ =
        toml::read_string(multiline_literal_string_);
    BOOST_CHECK_EQUAL(multiline_literal_string_result_, multiline_literal_string_str);
}

BOOST_AUTO_TEST_CASE(test_read_simple_value)
{
    const std::string integer_str("100_000");
    std::istringstream integer(integer_str);
    const std::string integer_result = toml::read_simple_value(integer);
    BOOST_CHECK_EQUAL(integer_result, integer_str);

    const std::string integer_str_("100_000, hoge");
    std::istringstream integer_(integer_str_);
    const std::string integer_result_ = toml::read_simple_value(integer_);
    BOOST_CHECK_EQUAL(integer_result_, integer_str);
}
