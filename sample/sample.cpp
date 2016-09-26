#include "toml.hpp"
#include <fstream>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "usage: ./sample sample.toml" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    try
    {

    toml::Data data = toml::parse(ifs);

    std::string title = toml::get<toml::String>(data.at("title"));
    std::cout << "title = " << title << std::endl;

    toml::Table table = toml::get<toml::Table>(data.at("table"));
    std::cout << "[table]" << std::endl;

    bool boolean = toml::get<toml::Boolean>(table.at("boolean"));
    std::cout << "boolean = " << boolean << std::endl;

    std::int_least64_t integer = toml::get<toml::Integer>(table.at("integer"));
    std::cout << "integer = " << integer << std::endl;

    double floating_point = toml::get<toml::Float>(table.at("float"));
    std::cout << "float = " << floating_point << std::endl;

    std::string basic_string = toml::get<toml::String>(table.at("basic_string"));
    std::cout << "basic_string = " << basic_string << std::endl;

    std::string multi_line_string = toml::get<toml::String>(table.at("multi_line_string"));
    std::cout << "multi_line_string = " << multi_line_string << std::endl;

    std::vector<std::int_least64_t> array = toml::get<toml::Array<toml::Integer>>(table.at("array"));
    std::cout << "array = [";
    for(auto item : array)
        std::cout << item << ", ";
    std::cout << "]" << std::endl;

    std::vector<std::vector<std::int_least64_t>> array_of_array =
        toml::get<toml::Array<toml::Array<toml::Integer>>>(table.at("array_of_array"));

    std::cout << "array_of_array = [";
    for(auto ar : array_of_array)
    {
        std::cout << "[";
        for(auto item : ar)
            std::cout << item << ", ";
        std::cout << "], ";
    }
    std::cout << "]" << std::endl;

    toml::Table inline_table = toml::get<toml::Table>(table.at("inline_table"));
    std::cout << "[table.inline_table]" << std::endl;

    bool a = toml::get<toml::Boolean>(inline_table.at("a"));
    std::cout << "a = " << a << std::endl;
    std::string b = toml::get<toml::String>(inline_table.at("b"));
    std::cout << "b = " << b << std::endl;
    std::vector<std::string> c = toml::get<toml::Array<toml::String>>(inline_table.at("c"));
    std::cout << "c = [";
    for(auto item : c)
        std::cout << item << ", ";
    std::cout << "]" << std::endl;

    std::vector<toml::Table> inline_array_of_table = 
        toml::get<toml::Array<toml::Table>>(table.at("inline_array_of_table"));

    std::cout << "inline_array_of_table = {id = [";
    for(auto item : inline_array_of_table)
        std::cout << toml::get<toml::Integer>(item.at("id")) << ", ";
    std::cout << "]}" << std::endl;

    toml::Table table2 = toml::get<toml::Table>(data.at("table2"));
    std::cout << "[table2]" << std::endl;
    std::string basic_string2 = toml::get<toml::String>(table2.at("basic_string"));
    std::cout << "basic_string = " << basic_string2 << std::endl;

    std::vector<toml::Table> array_of_table =
        toml::get<toml::Array<toml::Table>>(data.at("array_of_table"));
    std::cout << "array_of_table = {index = [";
    for(auto item : array_of_table)
        std::cout << toml::get<toml::Integer>(item.at("index")) << ", ";
    std::cout << "]}" << std::endl;

    }// try
    catch(std::exception& except)
    {
        std::cerr << "exception thrown. what = " << except.what() << std::endl;
        return 1;
    }

    return 0;
}
