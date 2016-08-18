#ifndef TOML_STRING_UTILITY
#define TOML_STRING_UTILITY
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace toml
{

template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove(std::basic_string<charT, traits, alloc> str, const charT c)
{
    return std::basic_string<charT, traits, alloc>(
            str.begin(), std::remove(str.begin(), str.end(), c));
}

template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove_indent(const std::basic_string<charT, traits, alloc>& str)
{
    std::basic_istringstream<charT, traits, alloc> iss(str);
    iss >> std::ws;
    std::basic_string<charT, traits, alloc> retval;
    std::getline(iss, retval);
    return retval;
}

template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove_extraneous_whitespace(const std::basic_string<charT, traits, alloc>& str)
{
    std::basic_string<charT, traits, alloc> rev(str.rbegin(), str.rend());
    std::basic_istringstream<charT, traits, alloc> iss(rev);
    iss >> std::ws;
    std::basic_string<charT, traits, alloc> ret;
    std::getline(iss, ret);
    return std::basic_string<charT, traits, alloc>(ret.rbegin(), ret.rend());
}

template<typename charT, typename traits, typename alloc>
std::vector<std::basic_string<charT, traits, alloc> >
split(const std::basic_string<charT, traits, alloc>& str, const charT delim)
{
    std::istringstream iss(str);
    std::vector<std::basic_string<charT, traits, alloc> > retval;
    retval.reserve(1 + std::count(str.begin(), str.end(), delim));
    std::basic_string<charT, traits, alloc> temp;
    while(std::getline(iss, temp, delim))
    {
        retval.push_back(temp);
    }
    return retval;
}

template<typename charT, typename traits, typename alloc>
std::size_t
count(const std::basic_string<charT, traits, alloc>& str, const charT c)
{
    return std::count(str.begin(), str.end(), c);
}

template<typename charT, typename traits, typename alloc>
std::size_t
count(const std::basic_string<charT, traits, alloc>& str,
      const std::basic_string<charT, traits, alloc>& sub)
{
    std::size_t num = 0;
    int diff = str.size() - sub.size();
    for(std::size_t i=0; i <= diff; ++i)
        if(str.substr(i, sub.size()) == sub) ++num;
    return num;
}

template<typename charT, typename traits, typename alloc>
bool
is_closed(const std::basic_string<charT, traits, alloc>& str,
          const charT open, const charT close)
{
    int counter = 0;
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
             if(*iter == open)  ++counter;
        else if(*iter == close) --counter;
    return (counter == 0);
}


}//toml

#endif /* TOML_STRING_UTILITY */
