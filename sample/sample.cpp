#include "parser.hpp"
#include "get.hpp"
#include <fstream>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "usage: ./sample sample.toml" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    toml::Data data = toml::parse(ifs);

    std::string title = toml::get<toml::String>(data.at("title"));
    std::cout << "title = " << title << std::endl;

    toml::Table table = toml::get<toml::Table>(data.at("table"));
    std::string a = toml::get<toml::String>(table.at("a"));
    std::string b = toml::get<toml::String>(table.at("b"));
    std::string c = toml::get<toml::String>(table.at("c"));
    std::string d = toml::get<toml::String>(table.at("d"));
    std::cout << "table" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;

    return 0;
}
