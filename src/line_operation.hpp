#ifndef TOML_STRING_UTILITY
#define TOML_STRING_UTILITY
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace toml
{

/*! @breaf remove all charactor in string and return the string.  *
 *  @param str remove charactor from this string.                 *
 *  @param c   remove this charactor.                             *
 *  because str is const reference, cannot use std::remove.       */
template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove(const std::basic_string<charT, traits, alloc>& str, const charT c)
{
    std::basic_string<charT, traits, alloc> retval;
    for(typename std::basic_string<charT, traits, alloc>::const_iterator
            iter = str.begin(); iter != str.end(); ++iter)
    {
        if(*iter != c) retval += *iter;
    }
    return retval;
}

/*! @breaf remove all whitespaces or tabs at the front of string. *
 *  @param str remove whitespaces from this string.               */
template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove_indent(const std::basic_string<charT, traits, alloc>& str)
{
    typename std::basic_string<charT, traits, alloc>::const_iterator
        iter = str.begin();
    while(iter != str.end())
    {
        if(!(*iter == ' ' || *iter == '\t')) break;
        ++iter;
    }
    return std::basic_string<charT, traits, alloc>(iter, str.end());
}

/*! @breaf remove all whitespaces or tabs at the end of string. *
 *  @param str remove whitespaces from this string.             */
template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
remove_extraneous_whitespace(const std::basic_string<charT, traits, alloc>& str)
{
    typename std::basic_string<charT, traits, alloc>::const_reverse_iterator
        iter = str.rbegin();
    while(iter != str.rend())
    {
        if(!(*iter == ' ' || *iter == '\t')) break;
        ++iter;
    }
    const std::basic_string<charT, traits, alloc> rev(iter, str.rend());
    return std::basic_string<charT, traits, alloc>(rev.rbegin(), rev.rend());
}

/*! @breaf count the number of charactor in string. *
 *  @param str count charactor in this string.      *
 *  @param c   count number of this charactor.      */
template<typename charT, typename traits, typename alloc>
std::size_t
count(const std::basic_string<charT, traits, alloc>& str, const charT c)
{
    return std::count(str.begin(), str.end(), c);
}

/*! @breaf count the number of substring in a string. *
 *  @param str count substring in this string.        *
 *  @param sub count number of this string.           */
template<typename charT, typename traits, typename alloc>
std::size_t
count(const std::basic_string<charT, traits, alloc>& str,
      const std::basic_string<charT, traits, alloc>& sub)
{
    std::size_t num = 0;
    const int diff = str.size() - sub.size();
    for(int i=0; i <= diff; ++i)
        if(str.substr(i, sub.size()) == sub) ++num;
    return num;
}

/*! @breaf split the string with a delimiter and return vector of string.     *
 *  @param str   split this string.                                           *
 *  @param delim split at this delimiter(return value does not include delim) *
 *  (ex.) split("this, is, example", ',') ==> ["this" " is" " example"]       */
template<typename charT, typename traits, typename alloc>
std::vector<std::basic_string<charT, traits, alloc> >
split(const std::basic_string<charT, traits, alloc>& str, const charT delim)
{
    std::vector<std::basic_string<charT, traits, alloc> > retval;
    retval.reserve(1 + count(str, delim));
    std::istringstream iss(str);
    std::basic_string<charT, traits, alloc> temp;
    while(std::getline(iss, temp, delim))
    {
        retval.push_back(temp);
    }
    return retval;
}

/*! @breaf check whether the string is closed with a kind of brace. *
 *  @param str   check this string.                                 *
 *  @param open  open symbol. for example, '['                      *
 *  @param close close symbol. for example, ']'                     */
template<typename charT, typename traits, typename alloc>
bool
is_closed(const std::basic_string<charT, traits, alloc>& str,
          const charT open, const charT close)
{
    int counter = 0;
    for(typename std::basic_string<charT, traits, alloc>::const_iterator
            iter = str.begin(); iter != str.end(); ++iter)
             if(*iter == open)  ++counter;
        else if(*iter == close) --counter;
    return (counter == 0);
}

}//toml

#endif /* TOML_STRING_UTILITY */
