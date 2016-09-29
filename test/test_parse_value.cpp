#define BOOST_TEST_MODULE "test_parse_value"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "../src/toml_parser.hpp"
#include "../src/get.hpp"

BOOST_AUTO_TEST_CASE(test_parse_as_Boolean)
{
    const std::string true_value("true");
    toml::ValueBase val1 = toml::parse_boolean_value(true_value);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Boolean> >(val1)->value == true);

    const std::string false_value("false");
    toml::ValueBase val2 = toml::parse_boolean_value(false_value);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Boolean> >(val2)->value == false);
}

BOOST_AUTO_TEST_CASE(test_parse_as_Integer)
{
    const std::string simple("123");
    toml::ValueBase val1 = toml::parse_integer_value(simple);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val1)->value == 123);

    const std::string explicit_positive("+123");
    toml::ValueBase val2 = toml::parse_integer_value(explicit_positive);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val2)->value == 123);

    const std::string negative("-123");
    toml::ValueBase val3 = toml::parse_integer_value(negative);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val3)->value == -123);

    const std::string delimiter("100_00_00");
    toml::ValueBase val4 = toml::parse_integer_value(delimiter);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val4)->value == 1000000);

    const std::string explicit_positive_delimiter("+100_00_00");
    toml::ValueBase val5 = toml::parse_integer_value(explicit_positive_delimiter);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val5)->value == 1000000);

    const std::string negative_delimiter("-100_00_00");
    toml::ValueBase val6 = toml::parse_integer_value(negative_delimiter);
    BOOST_CHECK(toml::dynamic_pointer_cast<toml::typed_value<toml::Integer> >(val6)->value == -1000000);
}

BOOST_AUTO_TEST_CASE(test_parse_as_Float)
{
    const std::string dot_simple("1.23");
    const double val1 = toml::get<toml::Float>(toml::parse_float_value(dot_simple));
    BOOST_CHECK(val1 == 1.23);

    const std::string e_simple("1e2");
    const double val2 = toml::get<toml::Float>(toml::parse_float_value(e_simple));
    BOOST_CHECK(val2 == 1.0e2);
    const std::string E_simple("1E2");
    const double val3 = toml::get<toml::Float>(toml::parse_float_value(E_simple));
    BOOST_CHECK(val3 == 1.0e2);
    const std::string dote_simple("1.E2");
    const double val4 = toml::get<toml::Float>(toml::parse_float_value(dote_simple));
    BOOST_CHECK(val4 == 1e2);
    const std::string dotE_simple("1.E2");
    const double val5 = toml::get<toml::Float>(toml::parse_float_value(dotE_simple));
    BOOST_CHECK(val5 == 1e2);

    const std::string dot_explicit_positive("+1.23");
    const double val6 = toml::get<toml::Float>(toml::parse_float_value(dot_explicit_positive));
    BOOST_CHECK(val6 == 1.23);
    const std::string dot_negative("-1.23");
    const double val7 = toml::get<toml::Float>(toml::parse_float_value(dot_negative));
    BOOST_CHECK(val7 == -1.23);
    const std::string e_explicit_positive("+1e2");
    const double val8 = toml::get<toml::Float>(toml::parse_float_value(e_explicit_positive));
    BOOST_CHECK(val8 == 1e2);
    const std::string e_negative("-1e2");
    const double val9 = toml::get<toml::Float>(toml::parse_float_value(e_negative));
    BOOST_CHECK(val9 == -1e2);
    const std::string E_positive("+1E2");
    const double val10 = toml::get<toml::Float>(toml::parse_float_value(E_positive));
    BOOST_CHECK(val10 == 1e2);
    const std::string E_negative("-1E2");
    const double val11 = toml::get<toml::Float>(toml::parse_float_value(E_negative));
    BOOST_CHECK(val11 == -1e2);
    const std::string dote_explicit_positive("+1.e2");
    const double val12 = toml::get<toml::Float>(toml::parse_float_value(dote_explicit_positive));
    BOOST_CHECK(val12 == 1.e2);
    const std::string dote_negative("-1.e2");
    const double val13 = toml::get<toml::Float>(toml::parse_float_value(dote_negative));
    BOOST_CHECK(val13 == -1.e2);
    const std::string dotE_explicit_positive("+1.E2");
    const double val14 = toml::get<toml::Float>(toml::parse_float_value(dotE_explicit_positive));
    BOOST_CHECK(val14 == 1.e2);
    const std::string dotE_negative("-1.E2");
    const double val15 = toml::get<toml::Float>(toml::parse_float_value(dotE_negative));
    BOOST_CHECK(val15 == -1.e2);

    const std::string delimiter("10_000.00");
    const double val16 = toml::get<toml::Float>(toml::parse_float_value(delimiter));
    BOOST_CHECK(val16 == 10000.00);
    const std::string e_delimiter("10_000e1");
    const double val17 = toml::get<toml::Float>(toml::parse_float_value(e_delimiter));
    BOOST_CHECK(val17 == 10000e1);
    const std::string E_delimiter("10_000e1");
    const double val18 = toml::get<toml::Float>(toml::parse_float_value(E_delimiter));
    BOOST_CHECK(val18 == 10000e1);
    const std::string dotE_delimiter("10_000.0E1");
    const double val19 = toml::get<toml::Float>(toml::parse_float_value(dotE_delimiter));
    BOOST_CHECK(val19 == 10000.0e1);
    const std::string dote_delimiter("10_000.0e1");
    const double val20 = toml::get<toml::Float>(toml::parse_float_value(dote_delimiter));
    BOOST_CHECK(val20 == 10000.0e1);

    const std::string explicit_positive_delimiter("+10_000.00");
    const double val21 = toml::get<toml::Float>(toml::parse_float_value(explicit_positive_delimiter));
    BOOST_CHECK(val21 == 10000.00);
    const std::string e_explicit_positive_delimiter("+10_000E1");
    const double val22 = toml::get<toml::Float>(toml::parse_float_value(e_explicit_positive_delimiter));
    BOOST_CHECK(val22 == 10000e1);
    const std::string E_explicit_positive_delimiter("+10_000e1");
    const double val23 = toml::get<toml::Float>(toml::parse_float_value(E_explicit_positive_delimiter));
    BOOST_CHECK(val23 == 10000e1);
    const std::string negative_delimiter("-10_000.00");
    const double val24 = toml::get<toml::Float>(toml::parse_float_value(negative_delimiter));
    BOOST_CHECK(val24 == -10000.00);
}

BOOST_AUTO_TEST_CASE(test_parse_as_String)
{
    const std::string str1("\"hoge\"");
    const std::string val1 = toml::get<toml::String>(toml::parse_string_value(str1));
    BOOST_CHECK_EQUAL(val1, "hoge");

    const std::string str2("\'hoge\'");
    const std::string val2 = toml::get<toml::String>(toml::parse_string_value(str2));
    BOOST_CHECK_EQUAL(val2, "hoge");

    const std::string str3("\"\"\"\nRoses are red\nViolets are blue\"\"\"");
    const std::string val3 = toml::get<toml::String>(toml::parse_string_value(str3));
    BOOST_CHECK_EQUAL(val3, "Roses are red\nViolets are blue");

    const std::string str4("\"\"\"\nThe quick brown \\\n\n\n   fox jumps over \\\n     the lazy dog.\"\"\"");
    const std::string val4 = toml::get<toml::String>(toml::parse_string_value(str4));
    BOOST_CHECK_EQUAL(val4, "The quick brown fox jumps over the lazy dog.");

    const std::string str5("\"\"\"\\\n    The quick brown \\\n    fox jumps over \\\n    the lazy dog.\\\n\"\"\"");
    const std::string val5 = toml::get<toml::String>(toml::parse_string_value(str5));
    BOOST_CHECK_EQUAL(val5, "The quick brown fox jumps over the lazy dog.");

    const std::string str6("\'\'\'I [dw]on't need \\d{2} apples\'\'\'");
    const std::string val6 = toml::get<toml::String>(toml::parse_string_value(str6));
    BOOST_CHECK_EQUAL(val6, "I [dw]on't need \\d{2} apples");

    const std::string str7("\'\'\'\nThe first newline is\ntrimmed in raw strings.\n   All other whitespaces\n   is preserved.\'\'\'");
    const std::string val7 = toml::get<toml::String>(toml::parse_string_value(str7));
    BOOST_CHECK_EQUAL(val7, "The first newline is\ntrimmed in raw strings.\n   All other whitespaces\n   is preserved.");
}

BOOST_AUTO_TEST_CASE(test_parse_as_Array)
{
    const std::string str1("[1, 2, 3]");
    const std::vector<toml::Integer> val1 =
#ifdef TOML_ENABLE_CXX11
        toml::get<toml::Array<toml::Integer>>(toml::parse_array_value(str1));
#else
        toml::get<toml::Array<toml::Integer>::type>(toml::parse_array_value(str1));
#endif
    BOOST_CHECK_EQUAL(val1.at(0), 1);
    BOOST_CHECK_EQUAL(val1.at(1), 2);
    BOOST_CHECK_EQUAL(val1.at(2), 3);

    const std::string str2(
            "[\"this\", \'is\', \'\'\'string\'\'\', \"\"\"array\"\"\"]");
    const std::vector<toml::String> val2 =
#ifdef TOML_ENABLE_CXX11
        toml::get<toml::Array<toml::String>>(toml::parse_array_value(str2));
#else
        toml::get<toml::Array<toml::String>::type>(toml::parse_array_value(str2));
#endif
    BOOST_CHECK_EQUAL(val2.at(0), std::string("this"));
    BOOST_CHECK_EQUAL(val2.at(1), std::string("is"));
    BOOST_CHECK_EQUAL(val2.at(2), std::string("string"));
    BOOST_CHECK_EQUAL(val2.at(3), std::string("array"));

    const std::string str3("[[1, 2, 3], [4, 5, 6], [7]]");
    const std::vector<std::vector<toml::Integer> > val3 =
#ifdef TOML_ENABLE_CXX11
        toml::get<toml::Array<toml::Array<toml::Integer>>>(toml::parse_array_value(str3));
#else
        toml::get<toml::Array<toml::Array<toml::Integer>::type>::type>(toml::parse_array_value(str3));
#endif
    BOOST_CHECK_EQUAL(val3.at(0).at(0), 1);
    BOOST_CHECK_EQUAL(val3.at(0).at(1), 2);
    BOOST_CHECK_EQUAL(val3.at(0).at(2), 3);
    BOOST_CHECK_EQUAL(val3.at(1).at(0), 4);
    BOOST_CHECK_EQUAL(val3.at(1).at(1), 5);
    BOOST_CHECK_EQUAL(val3.at(1).at(2), 6);
    BOOST_CHECK_EQUAL(val3.at(2).at(0), 7);

    const std::string str4(
            "[[\"this\", \'is\'], [\'\'\'nested\'\'\', \"string\", \"\"\"array\"\"\"]]");
    const std::vector<std::vector<toml::String> > val4 =
#ifdef TOML_ENABLE_CXX11
        toml::get<toml::Array<toml::Array<toml::String>>>(toml::parse_array_value(str4));
#else
        toml::get<toml::Array<toml::Array<toml::String>::type>::type>(toml::parse_array_value(str4));
#endif
    BOOST_CHECK_EQUAL(val4.at(0).at(0), std::string("this"));
    BOOST_CHECK_EQUAL(val4.at(0).at(1), std::string("is"));
    BOOST_CHECK_EQUAL(val4.at(1).at(0), std::string("nested"));
    BOOST_CHECK_EQUAL(val4.at(1).at(1), std::string("string"));
    BOOST_CHECK_EQUAL(val4.at(1).at(2), std::string("array"));
}
