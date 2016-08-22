#ifndef TOML_STREAM_OPERATION
#define TOML_STREAM_OPERATION
#include <istream>
#include <string>
#include <vector>
#include "exceptions.hpp"
#include "escape_sequence.hpp"
#include "line_operation.hpp"

namespace toml
{

template<typename charT>
bool is_bare_key_component(const charT c)
{
         if('a' <= c && c <= 'z') return true;
    else if('A' <= c && c <= 'Z') return true;
    else if('0' <= c && c <= '9') return true;
    else if(c == '_' || c == '-') return true;
    else return false;
}

template<typename charT>
bool is_whitespace(const charT c)
{
    return (c == ' ' || c == '\t');
}

/* @brief check current position is newline. do not change the reading position. *
 * the reason why there isn't const is iostate. peek() may change the iostate. */
template<typename charT, typename traits>
bool is_newline(const charT c, std::basic_istream<charT, traits>& is)
{
    if(c == '\n') return true;
    else if(c == '\r' && is.peek() == '\n') return true;
    else return false;
}

/* @brief check current position is newline. do not change the reading position. *
 * the reason why there isn't const is iostate. peek() may change the iostate. */
template<typename charT, typename traits>
bool is_escape_sequence(const charT c, std::basic_istream<charT, traits>& is)
{
    if(c != '\\') return false;
    switch(is.peek())
    {
        case 'b':  return true;
        case 't':  return true;
        case 'n':  return true;
        case 'f':  return true;
        case 'r':  return true;
        case '\"': return true;
        case '\\': return true;
        case 'u':  return true;
        case 'U':  return true;
        default:   return false;
    }
}

template<typename charT, typename traits>
std::basic_istream<charT, traits>&
skip_whitespace(std::basic_istream<charT, traits>& is)
{
    while(is_whitespace(is.peek())){is.ignore();}
    return is;
}

template<typename charT, typename traits>
std::basic_istream<charT, traits>&
skip_comment(std::basic_istream<charT, traits>& is)
{
    if(is.get() != '#') throw internal_error<charT>("not comment");
    while(is.get() != '\n'){/*do nothing */;}
    return is;
}

/* @brief read inline basic_string from is and return the string with quote. */
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_inline_basic_string(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> retval;

    charT charactor = is.get();
    if(charactor != '\"')
        throw std::invalid_argument("not basic string: " + charactor);
    retval += charactor;

    bool escape_flag = false;
    while(!is.eof())
    {
        charactor = is.get();
        retval += charactor;
        if(!escape_flag && charactor == '\"') break;
        else if(charactor == '\\') escape_flag = true;
        else                       escape_flag = false;
    }
    return unescape(retval);
}

/* @brief read inline literal_string from is and return the string with quote.*/
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_inline_literal_string(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> retval;
    charT charactor = is.get();
    if(charactor != '\'')
        throw std::internal_error<charT>("not string literal: " + charactor);
    retval += charactor;

    while(!is.eof())
    {
        charactor = is.get();
        retval += charactor;
        if(charactor == '\'') break;
    }
    return retval;
}

/* @brief read multiline basic_string and return the string with quotes. */
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_multiline_basic_string(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> raw;
    for(std::size_t i=0; i<3; ++i)
    {
        raw += is.get();
        if(raw.back() != '\"')
            throw std::internal_error<char>("not multi-line basic string");
    }

    bool quote_flag = false;
    // read until """
    while(!is.eof())
    {
        raw += is.get();
        if(raw.back() == '\"')
        {
            if(quote_flag && is.peek() == '\"')
            {
                raw += is.get();
                break;
            }
            quote_flag = true;
        }
    }
    if(raw.size() < 6)
        throw internal_error<char>("invalid multi-line string");

    // parse
    std::basic_istringstream<charT, traits> iss(raw);
    std::basic_string<charT, traits> parsed;
    while(!iss.eof())
    {
        bool CRflag = false;
        std::basic_string<charT, traits> line;
        std::getline(iss, line);
        if(line.back() == '\r')
        {
            CRflag = true;
            line = line.substr(0, line.size() - 1);
        }

        line = remove_indent(remove_extraneous_whitespace(line));
        if(line.back() == '\\')
        {
            line = line.substr(0, line.size() - 1);
        }
        else if(line == "\"\"\"")
        {
            ; //do noting
        }
        else
        {
            if(CRflag) line += '\r';
            line += '\n';
        }
        parsed += line;
    }
    return unescape(parsed);
}

/* @brief read multiline literal_string and return the string with quotes. */
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_multiline_literal_string(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> raw;
    for(std::size_t i=0; i<3; ++i)
    {
        raw += is.get();
        if(raw.back() != '\'')
            throw std::internal_error<char>("not multi-line literal string");
    }

    bool quote_flag = false;
    // read until '''
    while(!is.eof())
    {
        raw += is.get();
        if(raw.back() == '\'')
        {
            if(quote_flag && is.peek() == '\'')
            {
                raw += is.get();
                break;
            }
            quote_flag = true;
        }
    }
    if(raw.size() < 6)
        throw internal_error<char>("invalid multi-line literal");

    // parse
    std::basic_istringstream<charT, traits> iss(raw);
    std::basic_string<charT, traits> parsed;
    bool first_line = true;
    while(!iss.eof())
    {
        bool CRflag = false;
        std::basic_string<charT, traits> line;
        std::getline(iss, line);
        if(line.back() == '\r')
        {
            CRflag = true;
            line = line.substr(0, line.size() - 1);
        }

        if(first_line && remove_extraneous_whitespace(line) == "\'\'\'")
        {
            line = "\'\'\'";
        }
        else
        {
            if(CRflag) line += '\r';
            line += '\n';
        }
        parsed += line;
        first_line = false;
    }
    return parsed;
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_string_value(std::basic_istream<charT, traits>& is)
{
    const traits::pos_type current_pos = is.tellg();
    if(pos.get() == '\"')
    {
        if(pos.get() == '\"' && pos.get() == '\"')
        {
            is.seekg(current_pos);
            return read_multiline_basic_string(is);
        }
        else
        {
            is.seekg(current_pos);
            return read_inline_basic_string(is);
        }
    }
    else if(pos.get() == '\'')
    {
        if(pos.get() == '\'' && pos.get() == '\'')
        {
            is.seekg(current_pos);
            return read_multiline_literal_string(is);
        }
        else
        {
            is.seekg(current_pos);
            return read_inline_literal_string(is);
        }   
    }
    else
        throw internal_error<char>("not toml string value");
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_array(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> array_str;
    array_str += is.get();
    if(array_str.front() != '[')
        throw internal_error<charT>("not TOML Array value");
    while(!is.eof())
    {
        array_str += is.get();
        if(is_closed(array_str, '[', ']')) break;
    }
    return array_str;
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_inline_table(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> table_str;
    table_str += is.get();
    if(table_str.front() != '{')
        throw internal_error<charT>("not TOML Array value");
    while(!is.eof())
    {
        table_str += is.get();
        if(is_closed(table_str, '{', '}')) break;
    }
    return table_str;
}

// for Boolean, Integer, Float, Datetime
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_simple_value(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> value_str;
    while(!is.eof())
    {
        table_str += is.get();
        if(is.peek() == ' '  || is.peek() == '\t' || is.peek() == ',' ||
           is.peek() == '\n' || is.peek() == '\r' || is.peek() == '#' ||
           is.peek() == ']'  || is.peek() == '}') break;
    }
    return table_str;
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_value(std::basic_istream<charT, traits>& is)
{
    if(is.peek() == '\"' || is.peek() == '\'')
        return read_string_value(is);
    else if(is.peek() == '[')
        return read_array(is);
    else if(is.peek() == '{')
        return read_inline_table(is);
    else
        return read_simple_value(is);
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_bare_key(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> key;
    while(is_bare_key_component(is.peek()) && !is.eof())
    {
        key += is.get();
    }
    return key;
}

// assert (next statement is key-value pair)
template<typename charT, typename traits>
std::basic_string<charT, traits>
read_key(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> key_string;
    if(is.peek() == '\'' || is.peek() == '\"')
    {
        key_string = read_string_value(is);
    }
    else if(is_bare_key_component(is.peek()))
    {
        key_string = read_bare_key(is);
    }
    else throw syntax_error<charT, traits, alloc>("invalid key");

    return key_string;
}

template<typename charT, typename traits>
std::vector<std::basic_string<charT, traits> >
read_table_title(std::basic_istream<charT, traits>& is)
{
    std::vector<std::basic_string<charT, traits> > table_title;
    std::basic_string<charT, traits> title_key;
    if(is.peek() != '[')
        throw internal_error<charT>("not TOML table title");
    is.ignore();
    if(is.peek() == '[') is.ignore();
    while(!is.eof())
    {
        if(is.peek() ==  ']')
        {
            is.ignore();
            table_title.push_back(title_key);
            break;
        }
        else if(is.peek() == '.')
        {
            is.ignore();
            table_title.push_back(title_key);
            titile_key = read_key(is);
        }
        else
        {
            titile_key = read_key(is);
        }
    }
    return table_title;
}

template<typename charT, typename traits>
std::pair<std::basic_string<charT, traits>, std::basic_string<charT, traits> >
read_key_value_pair(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> key = read_key(is);
    skip_whitespace(is);
    if(is.get() != '=')
        throw syntax_error<charT>("missing = in key :" + key);
    skip_whitespace(is);
    std::basic_string<charT, traits> value = read_value(is);
    return std::make_pair(key, value);
}

}//toml

#endif /* TOML_STREAM_OPERATION */
