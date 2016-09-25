#include "toml.hpp"
#include <fstream>

using namespace toml;

int main()
{
    std::ifstream ifs("example.toml");
    if(!ifs.good())
    {
        std::cerr << "file open error: ./example.toml" << std::endl;
        return 1;
    }

    auto data = parse(ifs);

    ////////////////////////////////////////////////////////////////////////////
    // Table 
    auto table = get<Table>(data.at("table"));
    std::cout << "[table]" << std::endl;
    auto table_key = get<String>(table.at("key"));
    std::cout << "key = " << table_key << std::endl;

    auto x    = get<Table>(data.at("x"));
    std::cout << "[x]" << std::endl;
    auto xy   = get<Table>(x.at("y"));
    std::cout << "[x.y]" << std::endl;
    auto xyz  = get<Table>(xy.at("z"));
    std::cout << "[x.y.z]" << std::endl;
    auto xyzw = get<Table>(xyz.at("w"));
    std::cout << "[x.y.z.w]" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Inline Table 
    auto table_inline = get<Table>(table.at("inline"));
    std::cout << "[table.inline]" << std::endl;
    auto table_inline_name = get<Table>(table_inline.at("name"));
    std::cout << "[table.inline.name]" << std::endl;
    auto table_inline_name_first = get<String>(table_inline_name.at("first"));
    std::cout << "first = " << table_inline_name_first << std::endl;
    auto table_inline_name_last  = get<String>(table_inline_name.at("last"));
    std::cout << "last = " << table_inline_name_last << std::endl;
    auto table_inline_point = get<Table>(table_inline.at("point"));
    std::cout << "[table.inline.point]" << std::endl;
    auto table_inline_point_x  = get<Integer>(table_inline_point.at("x"));
    std::cout << "x = " << table_inline_point_x << std::endl;
    auto table_inline_point_y  = get<Integer>(table_inline_point.at("y"));
    std::cout << "y = " << table_inline_point_y << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // String 
    auto string_ = get<Table>(data.at("string"));
    std::cout << "[string]" << std::endl;
    auto string_basic = get<Table>(string_.at("basic"));
    std::cout << "[string.basic]" << std::endl;
    auto string_basic_basic = get<String>(string_basic.at("basic"));
    std::cout << "basic = " << string_basic_basic << std::endl;

    auto string_multiline = get<Table>(string_.at("multiline"));
    std::cout << "[string.multiline]" << std::endl;
    auto string_multiline_key1 = get<String>(string_multiline.at("key1"));
    std::cout << "key1 = " << string_multiline_key1 << std::endl;
    auto string_multiline_key2 = get<String>(string_multiline.at("key2"));
    std::cout << "key2 = " << string_multiline_key2 << std::endl;
    auto string_multiline_key3 = get<String>(string_multiline.at("key3"));
    std::cout << "key3 = " << string_multiline_key3 << std::endl;

    auto string_multiline_continued = get<Table>(string_multiline.at("continued"));
    std::cout << "[string.multiline.continued]" << std::endl;
    auto string_multiline_continued_key1 =
        get<String>(string_multiline_continued.at("key1"));
    std::cout << "key1 = " << string_multiline_continued_key1 << std::endl;
    auto string_multiline_continued_key2 =
        get<String>(string_multiline_continued.at("key2"));
    std::cout << "key2 = " << string_multiline_continued_key2 << std::endl;
    auto string_multiline_continued_key3 =
        get<String>(string_multiline_continued.at("key3"));
    std::cout << "key3 = " << string_multiline_continued_key3 << std::endl;

    auto string_literal = get<Table>(string_.at("literal"));
    std::cout << "[string.literal]" << std::endl;
    auto string_literal_winpath = get<String>(string_literal.at("winpath"));
    std::cout << "winpath = " << string_literal_winpath << std::endl;
    auto string_literal_winpath2 = get<String>(string_literal.at("winpath2"));
    std::cout << "winpath2 = " << string_literal_winpath2 << std::endl;
    auto string_literal_quoted = get<String>(string_literal.at("quoted"));
    std::cout << "quoted = " << string_literal_quoted << std::endl;
    auto string_literal_regex = get<String>(string_literal.at("regex"));
    std::cout << "regex = " << string_literal_regex << std::endl;

    auto string_literal_multiline = get<Table>(string_literal.at("multiline"));
    std::cout << "[string.literal.multiline]" << std::endl;
    auto string_literal_multiline_regex2 =
        get<String>(string_literal_multiline.at("regex2"));
    std::cout << "regex2 = " << string_literal_multiline_regex2 << std::endl;
    auto string_literal_multiline_lines =
        get<String>(string_literal_multiline.at("lines"));
    std::cout << "lines = " << string_literal_multiline_lines << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Integer 
    auto integer = get<Table>(data.at("integer"));
    std::cout << "[integer]" << std::endl;
    auto integer_key1 = get<Integer>(integer.at("key1"));
    std::cout << "key1 = " << integer_key1 << std::endl;
    auto integer_key2 = get<Integer>(integer.at("key2"));
    std::cout << "key2 = " << integer_key2 << std::endl;
    auto integer_key3 = get<Integer>(integer.at("key3"));
    std::cout << "key3 = " << integer_key3 << std::endl;
    auto integer_key4 = get<Integer>(integer.at("key4"));
    std::cout << "key4 = " << integer_key4 << std::endl;

    auto integer_underscores = get<Table>(integer.at("underscores"));
    std::cout << "[integer.underscores]" << std::endl;
    auto integer_underscores_key1 = get<Integer>(integer_underscores.at("key1"));
    std::cout << "key1 = " << integer_underscores_key1 << std::endl;
    auto integer_underscores_key2 = get<Integer>(integer_underscores.at("key2"));
    std::cout << "key2 = " << integer_underscores_key2 << std::endl;
    auto integer_underscores_key3 = get<Integer>(integer_underscores.at("key3"));
    std::cout << "key3 = " << integer_underscores_key3 << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Float
    auto float_ = get<Table>(data.at("float"));
    std::cout << "[float]" << std::endl;

    auto float_fractional = get<Table>(float_.at("fractional"));
    std::cout << "[float.fractional]" << std::endl;
    auto float_fractional_key1 = get<Float>(float_fractional.at("key1"));
    std::cout << "key1 = " << float_fractional_key1 << std::endl;
    auto float_fractional_key2 = get<Float>(float_fractional.at("key2"));
    std::cout << "key2 = " << float_fractional_key2 << std::endl;
    auto float_fractional_key3 = get<Float>(float_fractional.at("key3"));
    std::cout << "key3 = " << float_fractional_key3 << std::endl;
    
    auto float_exponent = get<Table>(float_.at("exponent"));
    std::cout << "[float.exponent]" << std::endl;
    auto float_exponent_key1 = get<Float>(float_exponent.at("key1"));
    std::cout << "key1 = " << float_exponent_key1 << std::endl;
    auto float_exponent_key2 = get<Float>(float_exponent.at("key2"));
    std::cout << "key2 = " << float_exponent_key2 << std::endl;
    auto float_exponent_key3 = get<Float>(float_exponent.at("key3"));
    std::cout << "key3 = " << float_exponent_key3 << std::endl;

    auto float_both = get<Table>(float_.at("both"));
    std::cout << "[float.both]" << std::endl;
    auto float_both_key = get<Float>(float_both.at("key"));
    std::cout << "key = " << float_both_key << std::endl;

    auto float_underscores = get<Table>(float_.at("underscores"));
    std::cout << "[float.underscores]" << std::endl;
    auto float_underscores_key1 = get<Float>(float_underscores.at("key1"));
    std::cout << "key1 = " << float_underscores_key1 << std::endl;
    auto float_underscores_key2 = get<Float>(float_underscores.at("key2"));
    std::cout << "key2 = " << float_underscores_key2 << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Boolean
    auto boolean = get<Table>(data.at("boolean"));
    std::cout << "[boolean]" << std::endl;
    auto boolean_True = get<Boolean>(boolean.at("True"));
    std::cout << "True = " << boolean_True << std::endl;
    auto boolean_False = get<Boolean>(boolean.at("False"));
    std::cout << "False = " << boolean_False << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Datetime
    auto datetime = get<Table>(data.at("datetime"));
//     std::cout << "[datetime]" << std::endl;
    auto datetime_key1 = get<Datetime>(datetime.at("key1"));
//     std::cout << "key1 = " << datetime_key1 << std::endl;
    auto datetime_key2 = get<Datetime>(datetime.at("key2"));
//     std::cout << "key2 = " << datetime_key2 << std::endl;
    auto datetime_key3 = get<Datetime>(datetime.at("key3"));
//     std::cout << "key3 = " << datetime_key3 << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Array
    auto array = get<Table>(data.at("array"));
    std::cout << "[array]" << std::endl;
    auto array_key1 = get<Array<Integer>>(array.at("key1"));
    std::cout << "key1 = [";
    for(auto x : array_key1) std::cout << x << ", ";
    std::cout << "]" << std::endl;
    auto array_key2 = get<Array<String>>(array.at("key2"));
    std::cout << "key2 = [";
    for(auto x : array_key2) std::cout << x << ", ";
    std::cout << "]" << std::endl;
    auto array_key3 = get<Array<Array<Integer>>>(array.at("key3"));
    std::cout << "key3 = [";
    for(auto x : array_key3)
    {
        std::cout << "[";
        for(auto y : x) std::cout << y << ", ";
        std::cout << "],";
    }
    std::cout << "]" << std::endl;

    auto array_key4 = get<Array<ValueBase>>(array.at("key4"));
    std::cout << "key4 = [";
    auto array_key4_subarray0 = get<Array<Integer>>(array_key4.at(0));
    std::cout << "[";
    for(auto x : array_key4_subarray0) std::cout << x << ", ";
    std::cout << "], ";
    auto array_key4_subarray1 = get<Array<String>>(array_key4.at(1));
    std::cout << "[";
    for(auto x : array_key4_subarray1) std::cout << x << ", ";
    std::cout << "]";
    std::cout << "]" << std::endl;

    auto array_key5 = get<Array<Integer>>(array.at("key5"));
    std::cout << "key5 = [";
    for(auto x : array_key5) std::cout << x << ", ";
    std::cout << "]" << std::endl;
    auto array_key6 = get<Array<Integer>>(array.at("key6"));
    std::cout << "key6 = [";
    for(auto x : array_key6) std::cout << x << ", ";
    std::cout << "]" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Array of Table
    auto products = get<Array<Table>>(data.at("products"));
    std::cout << "[[products]]" << std::endl;
    auto products0_name = get<String>(products.at(0).at("name"));
    std::cout << "name = " << products0_name << std::endl;
    auto products0_sku = get<Integer>(products.at(0).at("sku"));
    std::cout << "sku = " << products0_sku << std::endl;
    std::cout << "[[products]]" << std::endl;
    std::cout << "[[products]]" << std::endl;
    auto products2_name = get<String>(products.at(2).at("name"));
    std::cout << "name = " << products2_name << std::endl;
    auto products2_sku = get<Integer>(products.at(2).at("sku"));
    std::cout << "sku = " << products2_sku << std::endl;
    auto products2_color = get<String>(products.at(2).at("color"));
    std::cout << "color = " << products2_color << std::endl;

    auto fruit = get<Array<Table>>(data.at("fruit"));
    std::cout << "[[fruit]]" << std::endl;
    auto fruit0_name = get<String>(fruit.at(0).at("name"));
    std::cout << "name = " << fruit0_name << std::endl;
    auto fruit0_physical = get<Table>(fruit.at(0).at("physical"));
    std::cout << "[fruit.physical]" << std::endl;
    auto fruit0_physical_color = get<String>(fruit0_physical.at("color"));
    std::cout << "color = " << fruit0_physical_color << std::endl;
    auto fruit0_physical_shape = get<String>(fruit0_physical.at("shape"));
    std::cout << "shape = " << fruit0_physical_shape << std::endl;
    auto fruit0_variety = get<Array<Table>>(fruit.at(0).at("variety"));
    std::cout << "[[fruit.variety]]" << std::endl;
    auto fruit0_variety0_name = get<String>(fruit0_variety.at(0).at("name"));
    std::cout << "name = " << fruit0_variety0_name << std::endl;
    auto fruit0_variety1_name = get<String>(fruit0_variety.at(1).at("name"));
    std::cout << "name = " << fruit0_variety1_name << std::endl;

    std::cout << "[[fruit]]" << std::endl;
    auto fruit1_name = get<String>(fruit.at(1).at("name"));
    std::cout << "name = " << fruit1_name << std::endl;
    auto fruit1_variety = get<Array<Table>>(fruit.at(1).at("variety"));
    std::cout << "[[fruit.variety]]" << std::endl;
    auto fruit1_variety0_name = get<String>(fruit1_variety.at(0).at("name"));
    std::cout << "name = " << fruit1_variety0_name << std::endl;
 
    return 0;
}
