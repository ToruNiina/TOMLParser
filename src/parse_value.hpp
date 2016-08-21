#ifndef TOML_PARSE_VALUE
#define TOML_PARSE_VALUE
#include "definitions.hpp"
#include "toml_values.hpp"
#include "is.hpp"
#include <algorithm>

namespace toml
{

template<typename T, typename charT, typename traits, typename alloc>
struct parse_value_impl;

template<typename charT, typename traits, typename alloc>
std::shared_ptr<value_base>
parse_value(const std::basic_string<charT, traits, alloc>& str)
{
    if(is<Boolean>(str))
        return parse_value_impl<Boolean, charT, traits, alloc>::apply(str);
    else if(is<Integer>(str))
        return parse_value_impl<Integer, charT, traits, alloc>::apply(str);
    else if(is<Float>(str))
        return parse_value_impl<Float, charT, traits, alloc>::apply(str);
    else if(is<String>(str))
        return parse_value_impl<String, charT, traits, alloc>::apply(str);
    else if(is<Datetime>(str))
        return parse_value_impl<Datetime, charT, traits, alloc>::apply(str);
    else if(is<array_type>(str))
        return parse_value_impl<array_type, charT, traits, alloc>::apply(str);
    else if(is<table_type>(str))
        return parse_value_impl<table_type, charT, traits, alloc>::apply(str);
    else
        throw syntax_error<charT, traits, alloc>("unknown type " + str);
}

// ---------------------------- utility functions -----------------------------

// return iterator that points the brace that enclose the block.
// iter-> '['[1,2], [3,4], [5,6]']' <- retval
template<typename charT, typename traits, typename alloc>
typename std::basic_string<charT, traits, alloc>::const_iterator
find_bracket_close(
        typename std::basic_string<charT, traits, alloc>::const_iterator iter,
        const typename std::basic_string<charT, traits, alloc>::const_iterator end,
        const charT close)
{
    const charT open = *iter;
    int counter = 0;
    while(iter != end)
    {
             if(*iter == open)  ++counter;
        else if(*iter == close) --counter;
        if(counter == 0) break;
        ++iter;
    }
    return iter;
}

template<typename charT, typename traits, typename alloc>
typename std::basic_string<charT, traits, alloc>::const_iterator
find_string_end(
        typename std::basic_string<charT, traits, alloc>::const_iterator begin,
        const typename std::basic_string<charT, traits, alloc>::const_iterator end,
        const charT quat)
{
    bool esc = false;
    while(begin != end)
    {
             if(*begin == '\\')           esc = true;
        else if(*begin == quat && (!esc)) break;
        else                              esc = false;
        ++begin;
    }
    return begin;
}

template<typename charT, typename traits, typename alloc>
typename std::basic_string<charT, traits, alloc>::const_iterator
find_multi_string_end(
        typename std::basic_string<charT, traits, alloc>::const_iterator begin,
        const typename std::basic_string<charT, traits, alloc>::const_iterator end,
        const charT quat)
{
    typedef syntax_error<charT, traits, alloc> syntax_exception;
    if(std::distance(begin, end) < 3)
        throw syntax_exception("not multi string line " +
                std::basic_string<charT, traits, alloc>(begin, end));

    while(begin+2 != end)
    {
        if(*begin == quat && *(begin + 1) == quat && *(begin + 2) == quat)
            return begin + 2;
        ++begin;
    }
    return begin;
}

template<typename charT, typename traits, typename alloc>
typename std::basic_string<charT, traits, alloc>::const_iterator
find_value_end(
       const typename std::basic_string<charT, traits, alloc>::const_iterator begin, 
       const typename std::basic_string<charT, traits, alloc>::const_iterator end)
{
    typedef syntax_error<charT, traits, alloc> syntax_exception;
    typename std::basic_string<charT, traits, alloc>::const_iterator close;
    if(*begin == '[')
    {
        close = find_bracket_close<charT, traits, alloc>(begin, end, ']');
    }
    else if(*begin == '{')
    {
        close = find_bracket_close<charT, traits, alloc>(begin, end, '}');
    }
    else if(*begin == '\'')
    {
        if(std::distance(begin, end) > 3 &&
           *(begin+1) == '\'' && *(begin+2) == '\'')
        {
            close = find_multi_string_end<charT, traits, alloc>(begin+3, end, '\'');
        }
        else
        {
            close = find_string_end<charT, traits, alloc>(begin+1, end, '\'');
        }
    }
    else if(*begin == '\"')
    {
         if(std::distance(begin, end) > 3 &&
            *(begin+1) == '\"' && *(begin+2) == '\"')
        {
            close = find_multi_string_end<charT, traits, alloc>(begin+3, end, '\"');
        }
        else
        {
            close = find_string_end<charT, traits, alloc>(begin+1, end, '\"');
        }   
    }
    else // other values
    {
        close = begin;
        while(close != end)
        {
            if(*(close + 1) == ' ' || *(close + 1) == ',' ||
               *(close + 1) == ']' || *(close + 1) == '}') break;
            ++close;
        }
    }
    if(close == end) throw syntax_exception("not value: " + 
            std::basic_string<charT, traits, alloc>(begin, end));
    return close;
}

// split array string into vector of element strings.
template<typename charT, typename traits, typename alloc>
std::vector<std::basic_string<charT, traits, alloc>>
split_array(const std::basic_string<charT, traits, alloc>& str)
{
    using syntax_exception = syntax_error<charT, traits, alloc>;
    auto iter = str.cbegin();
    if(*iter != '[') throw syntax_exception("not array: " + str);
    ++iter;
    std::vector<std::basic_string<charT, traits, alloc> > splitted;
    while(iter != str.cend())
    {
        if(*iter == ']')
        {
            break;
        }
        else if(*iter == ' ')
        {
            ++iter;
        }
        else
        {
            const typename std::basic_string<charT, traits, alloc>::const_iterator
                next = find_value_end<charT, traits, alloc>(iter, str.end());
            std::basic_string<charT, traits, alloc> value(iter, next+1);
            splitted.push_back(value);
            iter = next + 1;
            while(*iter == ' ' || *iter == '\t'){++iter;}// remove 
            if(*iter != ',' && *iter != ']')
                throw syntax_exception("invalid array: " + str);
            ++iter;
        }
    }
    return splitted;
}

template<typename charT, typename traits, typename alloc>
std::vector<std::basic_string<charT, traits, alloc>>
split_table(const std::basic_string<charT, traits, alloc>& str)
{
// {foo = "some, string = sample", bar = [1, 2, 3], baz = {def = recursive}}
// ["foo = "some, string = sample"", "bar = [1, 2, 3]", "baz = {}"]
    typedef syntax_error<charT, traits, alloc> syntax_exception;
    typename std::basic_string<charT, traits, alloc>::const_iterator iter = str.begin();
    if(*iter != '{') throw syntax_exception(str);
    ++iter;
    std::vector<std::basic_string<charT, traits, alloc> > splitted;
    std::basic_string<charT, traits, alloc> temp_key;
    while(iter != str.end())
    {
        temp_key += *iter;
        if(*iter == '=')
        {
            ++iter;
            while(*iter == ' ' || *iter == '\t'){temp_key += *iter; ++iter;}
            const typename std::basic_string<charT, traits, alloc>::const_iterator
                next = find_value_end<charT, traits, alloc>(iter, str.end());
            const std::basic_string<charT, traits, alloc> value(iter, next+1);
            iter = next + 1;
            splitted.push_back(temp_key + value);
            temp_key.clear();
            while(*iter == ' ' || *iter == '\t'){++iter;}
            if(*iter != ',' && *iter != '}')
                throw syntax_exception("invalid table "+ str + " at " + *iter);
            else ++iter;
        }
        else
        {
            ++iter;
        }
    }
    return splitted;
}

template<typename charT, typename traits, typename alloc>
std::pair<std::basic_string<charT, traits, alloc>,
          std::basic_string<charT, traits, alloc> >
split_key_value(const std::basic_string<charT, traits, alloc>& str)
{
    // hoge = "key = value"
    std::basic_string<charT, traits, alloc> key(
            str.begin(), std::find(str.begin(), str.end(), '='));
    std::basic_string<charT, traits, alloc> value(
            std::find(str.begin(), str.end(), '=') + 1, str.end());
    key   = remove_extraneous_whitespace(remove_indent(key));
    value = remove_extraneous_whitespace(remove_indent(value));
    
    return std::make_pair(key, value);
}

// ---------------------------- implementation ---------------------------------

template<typename charT, typename traits, typename alloc>
struct parse_value_impl<Boolean, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<typed_value<Boolean>> val =
            std::make_shared<typed_value<Boolean>>();
             if(str == "true")  val->value = true;
        else if(str == "false") val->value = false;
        else throw internal_error<charT, traits, alloc>("not boolean type" + str);
        return val;
    }
};

template<typename charT, typename traits, typename alloc>
struct parse_value_impl<Integer, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<typed_value<Integer>> val =
            std::make_shared<typed_value<Integer>>();
        try{val->value = std::stoll(remove(str, '_'));}
        catch(std::exception& excpt)
        {throw internal_error<charT, traits, alloc>("not Integer type" + str);}
        return val;
    }
};

template<typename charT, typename traits, typename alloc>
struct parse_value_impl<Float, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<typed_value<Float>> val =
            std::make_shared<typed_value<Float>>();
        try{val->value = std::stod(remove(str, '_'));}
        catch(std::exception& excpt)
        {throw internal_error<charT, traits, alloc>("not Float type" + str);}
        return val;
    }
};

template<typename charT, typename traits, typename alloc>
struct parse_value_impl<String, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<typed_value<String>> val =
            std::make_shared<typed_value<String>>();
        if(str.substr(0,3) == "\'\'\'" || str.substr(0,3) == "\"\"\"")
            val->value = str.substr(3, str.size() - 6);
        else if(str.front() == '\'' || str.front() == '\"')
            val->value = str.substr(1, str.size() - 2);
        else throw internal_error<charT, traits, alloc>("not String type" + str);
        return val;
    }
};

// this implementation is extremely complecated...
template<typename charT, typename traits, typename alloc>
struct parse_value_impl<Datetime, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        typedef internal_error<charT, traits, alloc> internal_exception;
        try{
        //           1         2
        // 012345678901234567890123
        // patterns
        // yyyy-mm-dd
        // yyyy-mm-ddThh:mm:ss
        // yyyy-mm-ddThh:mm:ssZ
        // yyyy-mm-ddThh:mm:ss+09:00
        // yyyy-mm-ddThh:mm:ss.sss
        // yyyy-mm-ddThh:mm:ss.sssZ
        // yyyy-mm-ddThh:mm:ss.sss+09:00

        std::shared_ptr<typed_value<Datetime>> val =
            std::make_shared<typed_value<Datetime>>();
        std::basic_istringstream<charT, traits, alloc> iss(str);

        std::tm t;
        t.tm_year = std::stoi(get_word(iss, 4)) - 1900;// yyyy
        if(iss.get() != '-') throw internal_exception("-");// -
        t.tm_mon  = std::stoi(get_word(iss, 2)) - 1;   // mm
        if(iss.get() != '-') throw internal_exception("-");// -
        t.tm_mday = std::stoi(get_word(iss, 2));       // dd

        if(iss.eof())
        {
            t.tm_sec  = 0; t.tm_min  = 0; t.tm_hour = 0;
            val->value = std::chrono::system_clock::from_time_t(make_local(t));
            return val;
        }

        if(iss.get() != 'T') throw internal_exception("T");// T
        t.tm_hour = std::stoi(get_word(iss, 2));      // hh
        if(iss.get() != ':') throw internal_exception(":");// :
        t.tm_min  = std::stoi(get_word(iss, 2));      // mm
        if(iss.get() != ':') throw internal_exception(":");// :
        t.tm_sec  = std::stoi(get_word(iss, 2));      // ss

        bool secfrac = false;
        std::chrono::microseconds sfrac;
        if(iss.peek() == '.')
        {
            secfrac = true;
            std::basic_string<charT, traits, alloc> numf("0.");
            numf += get_number(iss);
            const double subsecond = std::stod(numf);
            sfrac = std::chrono::microseconds(static_cast<int>(subsecond * 1000000));
        }

        if(iss.eof()) // LOCAL
        {
            val->value = std::chrono::system_clock::from_time_t(make_local(t));
            if(secfrac) val->value += sfrac;
            return val;
        }
        else if(iss.peek() == 'Z') //GMT
        {
            val->value = std::chrono::system_clock::from_time_t(make_global(t));
            if(secfrac) val->value += sfrac;
            return val;
        }
        else if(iss.peek() == '+' || iss.peek() == '-') //GMT + offset
        {
            val->value = std::chrono::system_clock::from_time_t(make_global(t));
            if(secfrac) val->value += sfrac;
            apply_offset(val, iss);
            return val;
        }
        else 
        {
            throw internal_error<charT, traits, alloc>("not datetime");
        }

        }//try
        catch(std::exception& excpt)
        {
            throw internal_error<charT, traits, alloc>("cannot parse as Datetime " + str);
        }
    }

    static std::time_t make_global(std::tm t)
    {
        return std::mktime(&t);
    }

    static std::time_t make_local(std::tm t)
    {
        const std::time_t gtime = std::mktime(&t);
        return std::mktime(std::localtime(&gtime));
    }

    static void apply_offset(std::shared_ptr<typed_value<Datetime>>& val,
                             std::basic_istringstream<charT, traits, alloc>& iss)
    {
        if(iss.get() == '+')
        {
            val->value += std::chrono::hours(std::stoi(get_word(iss, 2)));
            if(iss.get() != ':') throw internal_error<charT, traits, alloc>(":");
            val->value += std::chrono::minutes(std::stoi(get_word(iss, 2)));
        }
        else
        {
            val->value -= std::chrono::hours(std::stoi(get_word(iss, 2)));
            if(iss.get() != ':') throw internal_error<charT, traits, alloc>(":");
            val->value -= std::chrono::minutes(std::stoi(get_word(iss, 2)));
        }
        return;
    }
};

template<typename charT, typename traits, typename alloc>
struct parse_value_impl<array_type, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<array_type> val = std::make_shared<array_type>();
        std::vector<std::basic_string<charT, traits, alloc>> splitted = 
            split_array(str);
        for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
            val->value.push_back(parse_value(*iter));
        return val;
    }
};

// TODO: parse inline table
template<typename charT, typename traits, typename alloc>
struct parse_value_impl<table_type, charT, traits, alloc>
{
    static std::shared_ptr<value_base>
    apply(const std::basic_string<charT, traits, alloc>& str)
    {
        std::shared_ptr<table_type> val = std::make_shared<table_type>();
        std::vector<std::basic_string<charT, traits, alloc> > splitted = 
            split_table(str);
        for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
        {
            std::pair<std::string, std::string> kv = split_key_value(*iter);
            val->value[kv.first] = parse_value(kv.second);
        }
        return val;
    }
};

}//toml

#endif /* TOML_PARSE_VALUE */
