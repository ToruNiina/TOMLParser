#ifndef TOML_PARSER
#define TOML_PARSER
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "exceptions.hpp"
#include "utility.hpp"
#include "toml_values.hpp"

namespace toml
{

// TODO: these are mockup
template<typename charT>
shared_ptr<value_base> parse_value(const std::basic_string<charT>& str)
{
    shared_ptr<typed_value<toml::String> > tmp = make_shared<typed_value<toml::String> >();
    tmp->value = str;
    return tmp;
}

template<typename charT>
std::basic_string<charT> parse_basic_string(const std::basic_string<charT>& str)
{
    return str;
}

template<typename charT>
std::basic_string<charT> parse_literal_string(const std::basic_string<charT>& str)
{
    return str;
}
// TODO end

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

template<typename charT>
std::basic_string<charT> read_array(std::basic_istream<charT>& is)
{//{{{
    if(is.peek() != '[') throw internal_error("read_array: invalid call");

    std::basic_string<charT> retval;
    std::size_t counter = 0;
    while(true)
    {
        if(is.peek() == '[')
            ++counter;
        else if(is.peek() == ']')
            --counter;
        retval += is.get();

        if(counter == 0) return retval;
        if(is.eof()) throw syntax_error("non-closed array");
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
        if(is.eof()) throw syntax_error("non-closed inline table");
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
            std::cerr << "is_multi_line " << multi_line << std::endl;
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
                    std::cerr << "now quote count = " << quote_count << std::endl;
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
                   throw syntax_error("read_basic_string: unexpected LF");
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
            throw syntax_error("read_literal_string: unexpected EOF");
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
                    throw syntax_error("read_literal_string: unexpected LF");
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
    while(not is_whitespace(is.peek()) &&
          not is_newline(is) &&
          not comment_starts(is.peek()))
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
std::basic_string<charT> parse_key(std::basic_istream<charT>& is)
{// {{{
    charT front = is.peek();
    std::cerr << "parse key: front = " << front << std::endl;
    if(is_bare_key_component(is.peek()))
    {
        return read_bare_key(is);
    }
    else if(is.peek() == '\'')
    {
        return parse_literal_string(read_literal_string(is));
    }
    else if(is.peek() == '\"')
    {
        return parse_basic_string(read_basic_string(is));
    }
    else
        throw syntax_error("invalid key");
}// }}}


template<typename charT>
std::pair<std::basic_string<charT>, shared_ptr<value_base> >
parse_key_value(std::basic_istream<charT>& is)
{// {{{
    std::basic_string<charT> key = parse_key(is);
    std::cerr << "parse_key_value key = (" << key << ") read" << std::endl;

    skip_whitespace(is);
    if(is.peek() != '=')
    {
        std::cerr << key << std::endl;
        std::cerr << "line " << line_number(is) << std::endl;
        throw syntax_error("no \'=\' after key in key-value line");
    }
    is.ignore();
    skip_whitespace(is);
    std::basic_string<charT> value = read_value(is);
    std::cerr << "parse_key_value: value = (" << value << ") read" << std::endl;

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
            throw syntax_error("parse_table_name: unexpected EOF");
        skip_whitespace(is);
        std::basic_string<charT> tmp = parse_key(is);
        std::cerr << "table name key (" << tmp << ") read. is_array = "<< is_array_of_table << std::endl;
        if(tmp.empty()) throw syntax_error("empty key");
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
                throw syntax_error("invalid array_of_table definition");
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
            std::cerr << "now line "<< line_number(is) << std::endl;
            std::cerr << "tmp = " << tmp << std::endl;
            throw syntax_error("invalid table definition");
        }
    }

    // after table name declared,
    // there are only whitespaces and/or comment until newline
    skip_whitespace(is);
    if(is_newline(is)) is.ignore();
    else if(comment_starts(is.peek())) skip_comment(is);
    else throw syntax_error("something exists after table declaration");

    return std::make_pair(is_array_of_table, name);
}//}}}

template<typename charT>
shared_ptr<value_base> make_nested_table(
        typename std::vector<std::basic_string<charT> >::const_iterator iter,
        typename std::vector<std::basic_string<charT> >::const_iterator end,
        const shared_ptr<value_base> contents, const bool is_array_of_table)
{// for [a.b.c.d] and [[a.b.c.d]] case. {{{
    if(iter == end)
    {
        if(is_array_of_table)
        {
            std::cerr << "create leaf of nested array_of_table." << std::endl;
            shared_ptr<array_type> aot = make_shared<array_type>();
            aot->value.push_back(contents);
            return aot;
        }
        else
        {
            std::cerr << "create leaf of nested table." << std::endl;
            return contents;
        }
    }
    else
    {
        std::cerr << "create node of nested table that name is " << *iter << std::endl;
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
                throw syntax_error("invalid array of table definition");
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
    shared_ptr<table_type<charT> > data = parse_table(is);

    while(!is.eof())
    {
        std::cerr << "now " << line_number(is) << std::endl;
        const std::pair<bool, std::vector<std::basic_string<charT> > >
            table_name = parse_table_name(is);
        shared_ptr<value_base> table_contents = parse_table(is);
        std::cerr << "parsing current table end" << std::endl;

//         shared_ptr<table_type<charT> > tmp_table =
//             dynamic_pointer_cast<table_type<charT> >(table_contents);
//         for(auto x : tmp_table->value)
//         {
//             std::cerr << "key  : " << x.first << std::endl;
//             shared_ptr<typed_value<String> > tmp_val = 
//                 dynamic_pointer_cast<typed_value<String> >(x.second);
//             std::cerr << "value: " << tmp_val->value << std::endl;
//         }

        search_and_make_nested_table<charT>(data, table_name.second.begin(),
                table_name.second.end(), table_contents, table_name.first);
    }
    std::cerr << "parse end" << std::endl;
    return data->value;                 
}// }}}

}
#endif /* TOML_PARSER */
