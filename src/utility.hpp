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

template<typename charT>
bool is_closed(const std::basic_string<charT>& str,
               const charT open, const charT close)
{
    if(str.front() != open || str.back() != close) return false;

    int counter = 0;
    for(typename std::basic_string<charT>::const_iterator
        iter = str.begin(); iter != str.end(); ++iter)
    {
        if(*iter == open) ++counter;
        else if(*iter == close) --counter;
    }
    return counter == 0;
}


}// toml
#endif /* TOML_UTILITY */
