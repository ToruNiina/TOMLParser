#ifndef TOML_UTILITY
#define TOML_UTILITY
#include <iostream>
#include <sstream>
#include <string>
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

template<typename charT>
int read_integer(std::basic_istream<charT>& is, std::size_t digit)
{
    int retval;
    std::basic_string<charT> str;
    for(std::size_t i=0; i<digit; ++i) str += is.get();
    std::basic_istringstream<charT> iss(str);
    iss >> retval;
    return retval;
}

template<typename charT>
std::basic_string<charT>
get_numbers(std::basic_istream<charT>& is)
{
    std::basic_string<charT> retval;
    while(!is.eof() && ('0' <= is.peek() && is.peek() <= '9'))
    {
        retval += is.get();
    }
    return retval;
}


}// toml
#endif /* TOML_UTILITY */
