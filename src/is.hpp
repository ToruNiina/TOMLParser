#ifndef TOML_IS
#define TOML_IS
#include "toml_values.hpp"
#include "line_operation.hpp"

namespace toml
{

template<typename T, typename charT, typename traits, typename alloc>
struct is_impl;

template<typename T, typename charT, typename traits, typename alloc>
bool is(const std::basic_string<charT, traits, alloc>& str)
{
    return is_impl<T, charT, traits, alloc>::apply(str);
}

template<typename charT, typename traits, typename alloc>
struct is_impl<Boolean, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        return ((str == "true") || (str == "false"));
    }
};

template<typename charT, typename traits, typename alloc>
struct is_impl<Integer, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        auto iter = str.cbegin();
        if(*iter == '+' || *iter == '-') ++iter;
        bool underscore = false;
        for(; iter != str.cend(); ++iter)
        {
            if(('0' <= *iter && *iter <= '9'))
            {
                underscore = false;
            }
            else if(*iter == '_')
            {// each underscore must be surrounded by at least one digit.
                if(underscore) return false;
                underscore = true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
};

template<typename charT, typename traits, typename alloc>
struct is_impl<Float, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        const std::size_t num_eE = std::count(str.cbegin(), str.cend(), 'e') + 
                                   std::count(str.cbegin(), str.cend(), 'E');
        if(1 < num_eE) return false;

        const std::size_t num_dot = std::count(str.cbegin(), str.cend(), '.');
        if(1 < num_dot) return false;

        auto iter = str.cbegin();
        if(*iter == '+' || *iter == '-') ++iter;

        bool flag_eE = false;
        bool underscore = false;
        for(; iter != str.cend(); ++iter)
        {
            if(('0' <= *iter && *iter <= '9'))
            {
                underscore = false;
            }
            else if(*iter == '_')
            {// each underscore must be surrounded by at least one digit.
                if(underscore) return false;
                underscore = true;
            }
            else if(*iter == 'e' || *iter == 'E')
            {
                if(underscore) return false;
                flag_eE = true; // this appear only once.
            }
            else if(*iter == '+' || *iter == '-')
            {
                if(underscore) return false;
                if(!flag_eE)   return false;// this is not begin.
            }
            else if(*iter == '.')
            {
                if(underscore) return false;
                if(!flag_eE)   return false;
            }
            else return false;
        }
        return true;
    }
};

template<typename charT, typename traits, typename alloc>
struct is_impl<String, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        if(str.front() == '\"' || str.front() == '\'')
        {
            if(str.front() == '\"' && str.back() == '\"') return true;
            if(str.front() == '\'' && str.back() == '\'') return true;

            if(str.substr(0, 3) == "\"\"\"" &&
               str.substr(str.size() - 3, 3) == "\"\"\"") return true;
            if(str.substr(0, 3) == "\'\'\'" &&
               str.substr(str.size() - 3, 3) == "\'\'\'") return true;
        }
        return false;
    }
};

template<typename charT, typename traits, typename alloc>
struct is_impl<Datetime, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
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

        if(str.size() < 10) return false;
        std::basic_istringstream<charT, traits, alloc> iss(str);
        if(!get_number_digit(iss, 4)) return false;
        if(iss.get() != '-')          return false;
        if(!get_number_digit(iss, 2)) return false;
        if(iss.get() != '-')          return false;
        if(!get_number_digit(iss, 2)) return false;

        if(iss.eof()) return true;
        else if(str.size() < 18) return false;

        if(iss.get() != 'T')          return false;
        if(!get_number_digit(iss, 2)) return false;
        if(iss.get() != ':')          return false;
        if(!get_number_digit(iss, 2)) return false;
        if(iss.get() != ':')          return false;
        if(!get_number_digit(iss, 2)) return false;
        if(iss.eof()) return true;

        const charT c = iss.get();
             if(c == 'Z') return iss.eof();
        else if(c == '+' || c == '-')
        {
            if(!get_number_digit(iss, 2)) return false;
            if(iss.get() != ':')          return false;
            if(!get_number_digit(iss, 2)) return false;
            return iss.eof();
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
            else if(n == 'Z')
            {
                return iss.eof();
            }
            else if(n == '+' || n == '-')
            {
                if(!get_number_digit(iss, 2)) return false;
                if(iss.get() != ':')          return false;
                if(!get_number_digit(iss, 2)) return false;
                return iss.eof();
            }
            else return false;
        }
        else return false;
    }

    static bool get_number_digit(
            std::basic_istringstream<charT, traits, alloc>& iss, 
            std::size_t l)
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

template<typename charT, typename traits, typename alloc>
struct is_impl<array_type, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        if(str.front() != '[' || str.back() != ']') return false;
        return is_closed(str, '[', ']');
    }
};

template<typename charT, typename traits, typename alloc>
struct is_impl<table_type, charT, traits, alloc>
{
    static bool apply(const std::basic_string<charT, traits, alloc>& str)
    {
        if(str.front() != '{' || str.back() != '}') return false;
        return is_closed(str, '{', '}');
    }
};



}// toml

#endif /* TOML_IS */
