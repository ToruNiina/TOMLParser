#ifndef TOML_PARSER
#define TOML_PARSER
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <ctime>
#include "exceptions.hpp"
#include "escape_sequence.hpp"
#include "toml_values.hpp"
#include "is.hpp"
#include "utility.hpp"

namespace toml
{
template<typename charT>
inline bool is_whitespace(const charT c)
{// {{{
    return c == ' ' || c == '\t';
}// }}}

template<typename charT>
inline void skip_whitespace(std::basic_istream<charT>& is)
{// {{{
    while(is_whitespace(is.peek()) && !is.eof()){is.ignore();}
    return;
}// }}}

// if true, is.peek() should be '\n'.
template<typename charT>
bool is_newline(std::basic_istream<charT>& is)
{// {{{
    if(is.peek() == '\n')
    {
        return true;
    }
    else if(is.peek() == '\r')
    {
        const charT CR = is.get();
        if(is.peek() == '\n') return true;
        else is.putback(CR);
    }
    return false;
}// }}}

template<typename charT>
inline bool comment_starts(const charT c)
{//{{{
    return c == '#';
}//}}}

template<typename charT>
void skip_comment(std::basic_istream<charT>& is)
{//{{{
    if(not comment_starts(is.peek()))
        throw internal_error("invalid skip_comment call");

    // ignore until newline begin
    while(not is_newline(is) && not is.eof()){is.ignore();}

    if(!is.eof())// here, next charactor is '\n'
        is.ignore();
    return;
}//}}}

template<typename charT>
void skip_ignorable(std::basic_istream<charT>& is)
{//{{{
    while(not is.eof())
    {
        if(is_whitespace(is.peek())) skip_whitespace(is);
        else if(is_newline(is)) is.ignore();
        else if(comment_starts(is.peek())) skip_comment(is);
        else break;
    }
    return ;
}//}}}

template<typename charT>
inline bool is_bare_key_component(const charT c)
{//{{{
         if('a' <= c && c <= 'z') return true;
    else if('A' <= c && c <= 'Z') return true;
    else if('0' <= c && c <= '9') return true;
    else if(c == '_' || c == '-') return true;
    return false;
}//}}}

template<typename charT>
std::basic_string<charT> read_bare_key(std::basic_istream<charT>& is)
{//{{{
    std::basic_string<charT> bare_key;
    while(is_bare_key_component(is.peek())){bare_key += is.get();}
    return bare_key;
}//}}}

// forward decl for read_array {{{
template<typename charT>
std::basic_string<charT> read_value(std::basic_istream<charT>& is);
// }}}

template<typename charT>
std::basic_string<charT> read_array(std::basic_istream<charT>& is)
{//{{{
    if(is.peek() != '[') throw internal_error("read_array: invalid call");

    std::basic_string<charT> retval;
    retval += is.get();
    while(true)
    {
        skip_ignorable(is);

        const std::basic_string<charT> tmp = read_value(is);

        retval += tmp;

        skip_ignorable(is);

        if(is.peek() == ',') retval += is.get();

        skip_ignorable(is);
        
        if(is.peek() == ']')
        {
            retval += is.get();
            return retval;
        }
        if(is.eof())
        {
            throw syntax_error("split_array: invalid array");
        }
    }
}//}}}

template<typename charT>
std::basic_string<charT> read_inline_table(std::basic_istream<charT>& is)
{//{{{
    if(is.peek() != '{')
        throw internal_error("read_inline_table: invalid call");

    std::basic_string<charT> retval;
    std::size_t counter = 0;
    while(true)
    {
        if(is.peek() == '{')
            ++counter;
        else if(is.peek() == '}')
            --counter;
        retval += is.get();

        if(counter == 0) return retval;
        if(is.eof())
        {
            throw syntax_error("non-closed inline table");
        }
    }
}//}}}

template<typename charT>
std::basic_string<charT> read_basic_string(std::basic_istream<charT>& is)
{// {{{
    if(is.peek() != '\"')
        throw internal_error("read_basic_string: invalid call");

    std::basic_string<charT> retval;
    bool multi_line = false;
    retval += is.get();
    if(is.peek() == '\"')
    {
        retval += is.get();
        if(is.peek() != '\"') return retval; //here, retval == ""
        multi_line = true;
        retval += is.get(); //here, retval == """
    }

    std::size_t quote_count = 0; // for the end of multi line string
    bool escaping = false;
    while(true)
    {
        if(is.eof())
        {
            throw syntax_error("read_basic_string: unexpected EOF");
        }
        switch(is.peek())
        {
            case '\\':
            {
                escaping = true;
                retval += is.get();
                quote_count = 0;
                break;
            }
            case '\"':
            {
                if(escaping)
                {
                    retval += is.get();
                    escaping = false;
                    break;
                }
                else if(multi_line)
                {
                    ++quote_count;
                    retval += is.get();
                    if(quote_count == 3) return retval;
                    break;
                }
                else
                {
                    retval += is.get();
                    return retval;
                }
            }
            case '\n':
            {
                if(not multi_line)
                {
                   throw syntax_error("read_basic_string: unexpected LF");
                }
                else
                    retval += is.get();
                escaping = false;
                quote_count = 0;
                break;
            }
            default:
            {
                escaping = false;
                quote_count = 0;
                retval += is.get();
                break;
            }
        }
    }
}// }}}

template<typename charT>
std::basic_string<charT> read_literal_string(std::basic_istream<charT>& is)
{//{{{
    if(is.peek() != '\'')
        throw internal_error("read_literal_string: invalid call");

    std::basic_string<charT> retval;
    bool multi_line = false;
    retval += is.get();
    if(is.peek() == '\'')
    {
        retval += is.get();
        if(is.peek() != '\'') return retval; //here, retval == ''
        multi_line = true;
        retval += is.get(); //here, retval == '''
    }

    std::size_t quote_count = 0; // for the end of multi line string
    while(true)
    {
        if(is.eof())
        {
            throw syntax_error("read_literal_string: unexpected EOF");
        }
        switch(is.peek())
        {
            case '\'':
            {
                if(multi_line)
                {
                    ++quote_count;
                    retval += is.get();
                    if(quote_count == 3) return retval;
                    break;
                }
                else
                {
                    retval += is.get();
                    return retval;
                }
            }
            case '\n':
            {
                if(not multi_line)
                {
                    throw syntax_error("read_literal_string: unexpected LF");
                }
                else
                    retval += is.get();
                quote_count = 0;
                break;
            }
            default:
            {
                quote_count = 0;
                retval += is.get();
                break;
            }
        }
    }
}// }}}

template<typename charT>
std::basic_string<charT> read_string(std::basic_istream<charT>& is)
{// {{{
    switch(is.peek())
    {
        case '\'':
            return read_literal_string(is);
        case '\"':
            return read_basic_string(is);
        default:
            throw internal_error("read_inline_table: invalid call");
    }
}// }}}

template<typename charT>
std::basic_string<charT> read_simple_value(std::basic_istream<charT>& is)
{// {{{
    std::basic_string<charT> retval;

    while(not is_whitespace(is.peek()) && not is_newline(is) &&
          not comment_starts(is.peek()) && not is.eof() &&
          is.peek() != ',' && is.peek() != ']' && is.peek() != '}')
    {
        retval += is.get();
    }
    return retval;
}// }}}

template<typename charT>
std::basic_string<charT> read_value(std::basic_istream<charT>& is)
{//{{{
    switch(is.peek())
    {
        case '[':
            return read_array(is);
        case '{':
            return read_inline_table(is);
        case '\'':
            return read_string(is);
        case '\"':
            return read_string(is);
        default:
            return read_simple_value(is);
    }
}//}}}

template<typename charT>
std::basic_string<charT>
parse_literal_string_key(const std::basic_string<charT>& str)
{// {{{
    if(str.size() > 6 && str.substr(0, 3) == "\'\'\'")
    {
        if(str.at(3) == '\n')
        {
            return std::basic_string<charT>(str.begin()+4, str.end()-3);
        }
        else if(str.at(3) == '\r' && str.at(4) == '\n')
        {
            return std::basic_string<charT>(str.begin()+5, str.end()-3);
        }
        else
        {
            return std::basic_string<charT>(str.begin()+3, str.end()-3);
        }
    }
    else
    {
        return std::basic_string<charT>(str.begin()+1, str.end() - 1);
    }
}// }}}

template<typename charT>
std::basic_string<charT>
parse_multi_line_basic_string_key(const std::basic_string<charT>& str)
{// {{{
    std::basic_istringstream<charT> iss(
            std::basic_string<charT>(str.begin()+3, str.end()-3));
    if(is_newline(iss)) iss.ignore();

    std::basic_string<charT> contents;
    while(!iss.eof())
    {
        if(iss.peek() == '\\')
        {
            const typename std::basic_istringstream<charT>::pos_type
                pos = iss.tellg();
            iss.ignore();
            if(is_newline(iss))
            {
                while(true)
                {
                    if(is_newline(iss)) iss.ignore();
                    else if(is_whitespace(iss.peek())) iss.ignore();
                    else break;
                }
            }
            else
            {
                iss.seekg(pos);
            }
        }
        const charT c = iss.get();
        if(c == std::char_traits<charT>::eof()) break;
        contents += c;
    }
    return unescape(contents);
}// }}}

template<typename charT>
std::basic_string<charT>
parse_inline_basic_string_key(const std::basic_string<charT>& str)
{// {{{
    const std::basic_string<charT> contents(str.begin() + 1, str.end() - 1);
    return unescape(contents);
}// }}}

template<typename charT>
std::basic_string<charT>
parse_basic_string_key(const std::basic_string<charT>& str)
{// {{{
    if(str.size() > 3 && str.substr(0, 3) == "\"\"\"")
        return parse_multi_line_basic_string_key(str);
    else 
        return parse_inline_basic_string_key(str);
}// }}}

template<typename charT>
std::basic_string<charT>
parse_string_key(const std::basic_string<charT>& str)
{// {{{
    if(str.at(0) == '\'')
        return parse_literal_string_key(str);
    else if(str.at(0) == '\"')
        return parse_basic_string_key(str);
    else throw internal_error("parse_string_key: invalid call");
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_string_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<typed_value<String> > val = make_shared<typed_value<String> >();
    val->value = parse_string_key(str);
    return val;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_boolean_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<typed_value<Boolean> > val = make_shared<typed_value<Boolean> >();
    if(str == "true") val->value = true;
    else if(str == "false") val->value = false;
    else throw internal_error("parse_boolean_value: invalid call");
    return val;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_integer_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<typed_value<Integer> > val = make_shared<typed_value<Integer> >();
    std::basic_string<charT> contents;
    for(typename std::basic_string<charT>::const_iterator
        iter = str.begin(); iter != str.end(); ++iter)
    {
        if(*iter != '_') contents += *iter;
    }

    std::basic_istringstream<charT> iss(contents);
    iss >> val->value;
    return val;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_float_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<typed_value<Float> > val = make_shared<typed_value<Float> >();
    std::basic_string<charT> contents;
    for(typename std::basic_string<charT>::const_iterator
        iter = str.begin(); iter != str.end(); ++iter)
    {
        if(*iter != '_') contents += *iter;
    }

    std::basic_istringstream<charT> iss(contents);
    iss >> val->value;
    return val;
}// }}}

template<typename charT>
void apply_offset(shared_ptr<typed_value<Datetime> >& val,
                  std::basic_istringstream<charT>& iss)
{// {{{
    switch(iss.get())
    {
        case '+':
        {
            val->value += toml::chrono::hours(read_integer(iss, 2));
            if(iss.get() != ':')
            {
                throw syntax_error("no : in datetime offset");
            }
            val->value += toml::chrono::minutes(read_integer(iss, 2));
            break;
        }
        case '-':
        {
            val->value -= toml::chrono::hours(read_integer(iss, 2));
            if(iss.get() != ':')
            {
                throw syntax_error("no : in datetime offset");
            }
            val->value -= toml::chrono::minutes(read_integer(iss, 2));
            break;
        }
        default:
            throw internal_error("apply_offset: invalid call");
    }
    return;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_datetime_value(const std::basic_string<charT>& str)
{// {{{
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

    typedef std::char_traits<charT> traits;
    toml::shared_ptr<typed_value<Datetime> > val =
        toml::make_shared<typed_value<Datetime> >();
    std::basic_istringstream<charT> iss(str);

    std::tm t;
    t.tm_year = read_integer(iss, 4) - 1900;
    if(iss.get() != '-')
    {
        throw syntax_error("no - in datetime");
    }
    t.tm_mon  = read_integer(iss, 2) - 1;
    if(iss.get() != '-')
    {
        throw syntax_error("no - in datetime");
    }
    t.tm_mday = read_integer(iss, 2);

    if(iss.peek() == traits::eof())
    {
        t.tm_sec  = 0; t.tm_min  = 0; t.tm_hour = 0;
        const std::time_t gtime = std::mktime(&t);
        val->value = toml::chrono::system_clock::from_time_t(
                std::mktime(std::localtime(&gtime)));
        return val;
    }

    if(iss.get() != 'T')
    {
        throw syntax_error("no T in Datetime");
    }
    t.tm_hour = read_integer(iss, 2);
    if(iss.get() != ':')
    {
        throw syntax_error(":");
    }
    t.tm_min  = read_integer(iss, 2);
    if(iss.get() != ':')
    {
        throw syntax_error(":");
    }
    t.tm_sec  = read_integer(iss, 2);

    bool secfrac = false;
    toml::chrono::microseconds sfrac;
    if(iss.peek() == '.')
    {
        iss.ignore();
        secfrac = true;
        std::basic_string<charT> numf("0.");
        numf += get_numbers(iss);
        std::basic_istringstream<charT> numfss(numf);
        double subsecond;
        numfss >> subsecond;
        sfrac = toml::chrono::microseconds(static_cast<int>(subsecond * 1000000));
    }

    if(iss.eof()) // LOCAL
    {
        const std::time_t gtime = std::mktime(&t);
        val->value = toml::chrono::system_clock::from_time_t(
                std::mktime(std::localtime(&gtime)));
        if(secfrac) val->value += sfrac;
        return val;
    }
    else if(iss.peek() == 'Z') //GMT
    {
        val->value = toml::chrono::system_clock::from_time_t(std::mktime(&t));
        if(secfrac) val->value += sfrac;
        return val;
    }
    else if(iss.peek() == '+' || iss.peek() == '-') //GMT + offset
    {
        val->value = toml::chrono::system_clock::from_time_t(std::mktime(&t));
        if(secfrac) val->value += sfrac;
        apply_offset(val, iss);
        return val;
    }
    else 
    {
        throw syntax_error("invalid datetime declaration");
    }

    }//try
    catch(std::exception& excpt)
    {
        std::cerr << "exception thrown. what = " << excpt.what() << std::endl;
        throw syntax_error("cannot parse datetime");
    }
}// }}}

// forward decl for parse_array_value and parse_table_value {{{
template<typename charT>
shared_ptr<value_base> parse_value(const std::basic_string<charT>&);
// }}}

template<typename charT>
std::vector<std::basic_string<charT> >
split_array(const std::basic_string<charT>& str)
{// {{{
    std::basic_istringstream<charT> iss(str);
    if(iss.peek() != '[') throw internal_error("split_array: invalid call");

    std::vector<std::basic_string<charT> > retval;
    iss.ignore();
    if(iss.peek() == ']') return retval;
    while(true)
    {
        skip_ignorable(iss);

        const std::basic_string<charT> tmp = read_value(iss);
        if(tmp.empty()) throw internal_error("split_array value is empty");
        retval.push_back(tmp);

        skip_ignorable(iss);

        if(iss.peek() == ',') iss.ignore();

        skip_ignorable(iss);

        if(iss.peek() == ']') break;
        if(iss.eof())
        {
            throw syntax_error("split_array: invalid array");
        }
    }
    return retval;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_array_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<array_type> retval = make_shared<array_type>();
    std::vector<std::basic_string<charT> > splitted = split_array(str);
    for(typename std::vector<std::basic_string<charT> >::const_iterator
        iter = splitted.begin(); iter != splitted.end(); ++iter)
    {
        retval->value.push_back(parse_value(*iter));
    }
    return retval;
}// }}}

// forward decl for parse_table_value {{{
template<typename charT>
std::basic_string<charT>
parse_key(std::basic_istream<charT>& is);

template<typename charT>
std::pair<std::basic_string<charT>, shared_ptr<value_base> >
parse_key_value(std::basic_istream<charT>& is);
// }}}

template<typename charT>
std::vector<std::basic_string<charT> >
split_table(const std::basic_string<charT>& str)
{// {{{
    std::basic_istringstream<charT> iss(str);
    if(iss.peek() != '{') throw internal_error("split_table: invalid call");

    iss.ignore();
    std::vector<std::basic_string<charT> > retval;
    skip_whitespace(iss);
    if(iss.peek() == '}') return retval;

    const std::basic_string<charT> equal(" = ");
    while(true)
    {
        skip_ignorable(iss);

        std::basic_string<charT> tmp_key = parse_key(iss);
        if(tmp_key.empty())
        {
            throw syntax_error("split_table: empty key");
        }

        skip_ignorable(iss);
        
        if(iss.peek() != '=')
        {
            throw syntax_error("split_table invalid inline table, no =");
        }
        iss.ignore();

        skip_ignorable(iss);

        std::basic_string<charT> tmp_value = read_value(iss);
        if(tmp_value.empty())
        {
            throw syntax_error("split_table: empty value");
        }

        retval.push_back(tmp_key + equal + tmp_value);

        skip_ignorable(iss);

        if(iss.peek() == ',') iss.ignore();

        skip_ignorable(iss);

        if(iss.peek() == '}') break;
        if(iss.eof())
        {
            throw syntax_error("split_table: invalid inline table");
        }
    }
    return retval;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_table_value(const std::basic_string<charT>& str)
{// {{{
    shared_ptr<table_type<charT> > retval = make_shared<table_type<charT> >();

    std::vector<std::basic_string<charT> > splitted = split_table(str);
    for(typename std::vector<std::basic_string<charT> >::const_iterator
        iter = splitted.begin(); iter != splitted.end(); ++iter)
    {
        std::basic_istringstream<charT> iss(*iter);
        std::pair<std::basic_string<charT>, shared_ptr<value_base> > kvpair =
            parse_key_value(iss);
        retval->value[kvpair.first] = kvpair.second;
    }
    return retval;
}// }}}

template<typename charT>
shared_ptr<value_base>
parse_value(const std::basic_string<charT>& str)
{// {{{
         if(is<Boolean>(str))    return parse_boolean_value<charT>(str);
    else if(is<Integer>(str))    return parse_integer_value<charT>(str);
    else if(is<Float>(str))      return parse_float_value<charT>(str);
    else if(is<String>(str))     return parse_string_value<charT>(str);
    else if(is<Datetime>(str))   return parse_datetime_value<charT>(str);
    else if(is<array_type>(str)) return parse_array_value<charT>(str);
    else if(is<table_type<charT> >(str)) return parse_table_value<charT>(str);
    else throw syntax_error("parse_value: unknown type");
}// }}}

template<typename charT>
std::basic_string<charT> parse_key(std::basic_istream<charT>& is)
{// {{{
    charT front = is.peek();
    if(is_bare_key_component(front))
    {
        return read_bare_key(is);
    }
    else if(is.peek() == '\'')
    {
        return parse_literal_string_key(read_literal_string(is));
    }
    else if(is.peek() == '\"')
    {
        return parse_basic_string_key(read_basic_string(is));
    }
    else
    {
        throw syntax_error("invalid key");
    }
}// }}}

template<typename charT>
std::pair<std::basic_string<charT>, shared_ptr<value_base> >
parse_key_value(std::basic_istream<charT>& is)
{// {{{
    std::basic_string<charT> key = parse_key(is);

    skip_whitespace(is);
    if(is.peek() != '=')
    {
        throw syntax_error("no \'=\' after key in key-value line");
    }
    is.ignore();
    skip_whitespace(is);
    std::basic_string<charT> value = read_value(is);

    return std::make_pair(key, parse_value(value));
}// }}}

template<typename charT>
shared_ptr<table_type<charT> >
parse_table(std::basic_istream<charT>& is)
{//{{{
    shared_ptr<table_type<charT> > tab = make_shared<table_type<charT> >();
    while(!is.eof())
    {
        skip_whitespace(is);

        if(is.eof())
        {
            break;
        }
        else if(is_newline(is))
        {
            is.ignore();
            continue;
        }
        else if(comment_starts(is.peek()))
        {
            skip_comment(is);
            continue;
        }
        else if(is.peek() == '[')
        {// another table definition
            return tab;
        }
        else
        {// key_value line
            std::pair<std::basic_string<charT>, shared_ptr<value_base> > kvpair =
                parse_key_value(is);
            tab->value[kvpair.first] = kvpair.second;
            continue;
        }
    }
    return tab;
}//}}}

// pairof(is_array_of_table, table names)
template<typename charT>
std::pair<bool, std::vector<std::basic_string<charT> > >
parse_table_name(std::basic_istream<charT>& is)
{//{{{
    if(is.peek() != '[')
        throw internal_error("parse_table_name: invalid call");
    is.ignore();

    bool is_array_of_table = false;
    std::vector<std::basic_string<charT> > name;

    if(is.peek() == '[')
    {
        is_array_of_table = true;
        is.ignore();
    }

    while(true)
    {
        if(is.eof())
        {
            throw syntax_error("parse_table_name: unexpected EOF");
        }
        skip_whitespace(is);
        std::basic_string<charT> tmp = parse_key(is);
        if(tmp.empty())
        {
            throw syntax_error("empty key");
        }
        name.push_back(tmp);
        skip_whitespace(is);
        if(is.peek() == '.')
        {
            is.ignore();
            continue;
        }
        else if(is.peek() == ']' && is_array_of_table)
        {
            is.ignore();
            if(is.peek() != ']')
            {
                throw syntax_error("invalid array_of_table definition");
            }
            is.ignore();
            break;
        }
        else if(is.peek() == ']')
        {
            is.ignore();
            break;
        }
        else
        {
            throw syntax_error("invalid table definition");
        }
    }

    // after table name declared,
    // there are only whitespaces and/or comment until newline
    skip_whitespace(is);
    if(is_newline(is)) is.ignore();
    else if(comment_starts(is.peek())) skip_comment(is);
    else 
    {
        throw syntax_error("something exists after table declaration");
    }

    return std::make_pair(is_array_of_table, name);
}//}}}

template<typename charT>
shared_ptr<value_base> make_nested_table(
        typename std::vector<std::basic_string<charT> >::const_iterator iter,
        typename std::vector<std::basic_string<charT> >::const_iterator end,
        const shared_ptr<value_base> contents, const bool is_array_of_table)
{// {{{
    if(iter == end)
    {
        if(is_array_of_table)
        {
            shared_ptr<array_type> aot = make_shared<array_type>();
            aot->value.push_back(contents);
            return aot;
        }
        else
        {
            return contents;
        }
    }
    else
    {
        shared_ptr<table_type<charT> > tmp = make_shared<table_type<charT> >();
        tmp->value[*iter] = make_nested_table<charT>(
                iter + 1, end, contents, is_array_of_table);
        return tmp;
    }
}//}}}

template<typename charT>
void search_and_make_nested_table(
        const shared_ptr<value_base> root,
        typename std::vector<std::basic_string<charT> >::const_iterator iter,
        typename std::vector<std::basic_string<charT> >::const_iterator end,
        const shared_ptr<value_base> contents, const bool is_array_of_table)
{// {{{
    if(iter == end)
    {
        shared_ptr<array_type> arr = dynamic_pointer_cast<array_type>(root);
        if(arr)
        {
            if(!is_array_of_table)
            {
                throw syntax_error("invalid array of table definition");
            }
            arr->value.push_back(contents);
            return;
        }
        throw syntax_error("array of table name conflicts");
    }
    else
    {
        shared_ptr<table_type<charT> > tab;

        shared_ptr<array_type> arr = dynamic_pointer_cast<array_type>(root);
        if(arr)
            tab = dynamic_pointer_cast<table_type<charT> >(arr->value.back());
        else
            tab = dynamic_pointer_cast<table_type<charT> >(root);

        if(tab->value.count(*iter) == 1)
            search_and_make_nested_table<charT>(tab->value[*iter], iter+1, end,
                                                contents, is_array_of_table);
        else
            tab->value[*iter] = make_nested_table<charT>(iter+1, end, contents,
                                                         is_array_of_table);
        return;
    }
}// }}}

template<typename charT>
toml::Data parse(std::basic_istream<charT>& is)
{// {{{
    try
    {
        shared_ptr<table_type<charT> > data = parse_table(is);
        while(!is.eof())
        {
            const std::pair<bool, std::vector<std::basic_string<charT> > >
                table_name = parse_table_name(is);
            shared_ptr<value_base> table_contents = parse_table(is);

            search_and_make_nested_table<charT>(data, table_name.second.begin(),
                    table_name.second.end(), table_contents, table_name.first);
        }
        return data->value;
    }
    catch(toml::exception& except)
    {
        std::cerr << "TOMLParser Error: line #" << line_number(is) << std::endl;
        throw;
    }
}// }}}

}
#endif /* TOML_PARSER */
