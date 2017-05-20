#ifndef TOML_IS
#define TOML_IS
#include "toml_values.hpp"
#include "utility.hpp"
#include <algorithm>

namespace toml
{

template<typename T, typename charT>
struct is_impl;

template<typename T, typename charT>
bool is(const std::basic_string<charT>& str)
{
    return is_impl<T, charT>::invoke(str);
}

template<typename charT>
struct is_impl<Boolean, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        return ((str == "true") || (str == "false"));
    }
};

template<typename charT>
struct is_impl<Integer, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        typename std::basic_string<charT>::const_iterator iter = str.begin();
        if(*iter == '+' || *iter == '-') ++iter;
        bool underscore = false;
        for(; iter != str.end(); ++iter)
        {
            if(('0' <= *iter && *iter <= '9'))
            {
                underscore = false;
            }
            else if(*iter == '_')
            {
                if(underscore) return false;
                else underscore = true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
};

template<typename charT>
struct is_impl<Float, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        const std::size_t num_eE = std::count(str.begin(), str.end(), 'e') + 
                                   std::count(str.begin(), str.end(), 'E');
        if(1 < num_eE) return false;

        const std::size_t num_dot = std::count(str.begin(), str.end(), '.');
        if(1 < num_dot) return false;

        typename std::basic_string<charT>::const_iterator iter = str.begin();
        if(*iter == '+' || *iter == '-') ++iter;

        bool flag_eE = false;
        bool underscore = false;
        for(; iter != str.end(); ++iter)
        {
            if(('0' <= *iter && *iter <= '9'))
            {
                underscore = false;
            }
            else if(*iter == '_')
            {
                if(underscore) return false;
                else underscore = true;
            }
            else if(*iter == 'e' || *iter == 'E')
            {
                if(underscore) return false;
                else flag_eE = true;
            }
            else if(*iter == '+' || *iter == '-')
            {
                if(underscore || !flag_eE) return false;
            }
            else if(*iter == '.')
            {
                if(underscore || flag_eE) return false;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
};

template<typename charT>
struct is_impl<String, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        if(str.substr(0, 3) == "\"\"\"")
            return str.substr(str.size() - 3, 3) == "\"\"\"";
        if(str.substr(0, 3) == "\'\'\'")
            return str.substr(str.size() - 3, 3) == "\'\'\'";

        typename std::basic_string<charT>::const_iterator iter = str.begin();
        if(*iter == '\'') // literal string
        {
            ++iter;
            while(iter + 1 != str.end())
            {
                if(*iter == '\'') return false;
                ++iter;
            }
            return true;
        }
        else if(*iter == '\"') // basic string
        {
            bool esc = false;
            ++iter;
            while(iter + 1 != str.end())
            {
                if(*iter == '\\')
                {
                    esc = true;
                }
                else if(*iter == '\"')
                {
                    if(!esc) return false;
                    esc = false;
                }
                else
                {
                    esc = false;
                }
                ++iter;
            }
            return true;
        
        }
        else return false;
    }
};

template<typename charT>
struct is_impl<Datetime, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        //           1         2
        // 012345678901234567890123
        // yyyy-mm-dd
        // yyyy-mm-ddThh:mm:ss
        // yyyy-mm-ddThh:mm:ssZ
        // yyyy-mm-ddThh:mm:ss+09:00
        // yyyy-mm-ddThh:mm:ss.sss
        // yyyy-mm-ddThh:mm:ss.sssZ
        // yyyy-mm-ddThh:mm:ss.sss+09:00
        typedef std::char_traits<charT> traits;

        if(str.size() < 10) return false;
        std::basic_istringstream<charT> iss(str);
        if(!read_number_digit(iss, 4))  return false;
        if(iss.get() != '-')            return false;
        if(!read_number_digit(iss, 2))  return false;
        if(iss.get() != '-')            return false;
        if(!read_number_digit(iss, 2))  return false;
        if(iss.peek() == traits::eof()) return true;

        if(iss.get() != 'T')            return false;
        if(!read_number_digit(iss, 2))  return false;
        if(iss.get() != ':')            return false;
        if(!read_number_digit(iss, 2))  return false;
        if(iss.get() != ':')            return false;
        if(!read_number_digit(iss, 2))  return false;
        if(iss.peek() == traits::eof()) return true;

        const charT c = iss.get();
        if(c == 'Z' || c == 'z')
        {
            return (iss.peek() == traits::eof());
        }
        else if(c == '+' || c == '-')
        {
            if(!read_number_digit(iss, 2)) return false;
            if(iss.get() != ':')           return false;
            if(!read_number_digit(iss, 2)) return false;
            return (iss.peek() == traits::eof());
        }
        else if(c == '.')
        {
            charT n = iss.get();
            if(n < '0' || '9' < n) return false;
            while('0' <= n && n <= '9' && (!iss.eof())){n = iss.get();}
            if(iss.eof())
            {
                return true;
            }
            else if(n == 'Z' || n == 'z')
            {
                return (iss.peek() == traits::eof());
            }
            else if(n == '+' || n == '-')
            {
                if(!read_number_digit(iss, 2)) return false;
                if(iss.get() != ':')           return false;
                if(!read_number_digit(iss, 2)) return false;
                return (iss.peek() == traits::eof());
            }
            else return false;
        }
        else return false;
    }

    static bool read_number_digit(std::basic_istringstream<charT>& iss, 
            const std::size_t l)
    {
        for(std::size_t i=0; i<l; ++i)
        {
            if(iss.eof()) return false;
            const charT c = iss.get();
            if(c < '0' || '9' < c) return false;
        }
        return true;
    }
};

template<typename charT>
struct is_impl<array_type, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        if(*(str.begin()) != '[' || *(str.rbegin()) != ']') return false;
        return true;
    }
};

template<typename charT>
struct is_impl<table_type<charT>, charT>
{
    static bool invoke(const std::basic_string<charT>& str)
    {
        if(*(str.begin()) != '{' || *(str.rbegin()) != '}') return false;
        return true;
    }
};



}// toml

#endif /* TOML_IS */
