#ifndef TOML_UTILITY
#define TOML_UTILITY
#include <iostream>
#include <cstddef>

namespace toml
{
    
template<typename charT>
std::size_t line_number(std::basic_istream<charT>& is)
{
    const typename std::basic_istream<charT>::pos_type position = is.tellg();
    is.seekg(is.beg);
    std::size_t lines = 1; // the first line is line 1.
    while(is.tellg() != position)
    {
        if(is.get() == '\n') ++lines;
    }
    is.seekg(position);
    return lines;
}

}// toml
#endif /* TOML_UTILITY */
