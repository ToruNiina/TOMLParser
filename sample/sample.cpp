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

    {
    toml::Table table = toml::get<toml::Table>(data.at("table"));
    bool a = toml::get<toml::Boolean>(table.at("a"));
    std::vector<std::int_least64_t> b = toml::get<toml::Array<toml::Integer>>(table.at("b"));
    std::string c = toml::get<toml::String>(table.at("c"));
    std::string d = toml::get<toml::String>(table.at("d"));
    std::cout << "table" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = [";
    for(auto x : b)
        std::cout << x << " ";
    std::cout << "]" << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
    }

    {
    toml::Table table = toml::get<toml::Table>(data.at("table2"));
    bool a = toml::get<toml::Boolean>(table.at("a"));
    std::vector<bool> b = toml::get<toml::Array<toml::Boolean>>(table.at("b"));
    std::string c = toml::get<toml::String>(table.at("c"));
    std::string d = toml::get<toml::String>(table.at("d"));
    std::cout << "table2" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = [";
    for(auto x : b)
        std::cout << x << " ";
    std::cout << "]" << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
    }
    }
    catch(std::exception& except)
    {
        std::cerr << "exception thrown. what = " << except.what() << std::endl;
        return 1;
    }

    return 0;
}
