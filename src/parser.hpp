#ifndef TOML_PARSER
#define TOML_PARSER
#include "definitions.hpp"
#include "exceptions.hpp"
#include "line_operation.hpp"
#include "toml_values.hpp"
#include "parse_to_value.hpp"

namespace toml
{

enum class LineKind
{
    TABLE_TITLE,          // (ex) [tablename]
    ARRAY_OF_TABLE_TITLE, // (ex) [[tablename]]
    KEY_VALUE,            // (ex) key = value
    UNKNOWN,
};

template<typename charT, typename traits, typename alloc>
inline std::basic_string<charT, traits, alloc>
remove_comment(const std::basic_string<charT, traits, alloc>& line)
{
    return std::basic_string<charT, traits, alloc>(line.cbegin(),
            std::find(line.cbegin(), line.cend(), '#'));
}

template<typename charT, typename traits>
std::basic_string<charT, traits>
read_line(std::basic_istream<charT, traits>& is)
{
    std::basic_string<charT, traits> line;
    std::getline(is, line);
    return remove_indent(remove_extraneous_whitespace(remove_comment(line)));
}

template<typename charT, typename traits, typename alloc>
std::basic_istream<charT, traits>&
putback_line(std::basic_istream<charT, traits>& is,
             const std::basic_string<charT, traits, alloc>& line)
{
    for(auto iter = line.crbegin(); iter != line.crend(); ++iter)
        is.putback(*iter);
    return is;
}

template<typename charT, typename traits, typename alloc>
LineKind
determine_line_kind(const std::basic_string<charT, traits, alloc>& line)
{
    if(line.front() == '[' && line.back() == ']')
    {
        if(line.size() >= 4)
            if(line.at(1) == '[' && line.at(line.size() - 2) == ']')
                return LineKind::ARRAY_OF_TABLE_TITLE;
        return LineKind::TABLE_TITLE;
    }
    else if(std::count(line.begin(), line.end(), '=') >= 1)
    {
        if(line.front() == '=')
            throw syntax_error("line starts with =");
        return LineKind::KEY_VALUE;
    }
    else
        return LineKind::UNKNOWN;// a part of multi-line value?
}

template<typename charT, typename traits, typename alloc>
std::basic_string<charT, traits, alloc>
extract_table_title(const std::basic_string<charT, traits, alloc>& line)
{
    if(line.front() == '[' && line.back() == ']')
    {
        if(line.size() >= 4)
            if(line.at(1) == '[' && line.at(line.size() - 2) == ']')
                return line.substr(2, line.size()-4);
        return line.substr(1, line.size()-2);
    }
    else
        throw internal_error("not table title line");
}

template<typename charT, typename traits, typename alloc>
bool
is_multi_line_value(const std::basic_string<charT, traits, alloc>& line)
{
    // multi-line string/string-literal, multi-line array.
    // multi-line inline table(!) is ignored
         if(count(line, std::basic_string<charT, traits, alloc>("\"\"\"")) == 1)
        return true;
    else if(count(line, std::basic_string<charT, traits, alloc>("\'\'\'")) == 1)
        return true;
    else if(line.front() == '[')
        return !is_closed(line, '[', ']');
    else
        return false;
}

template<typename charT, typename traits, typename alloc>
std::pair<std::string, std::shared_ptr<value_base>>
parse_key_value(const std::basic_string<charT, traits, alloc>& line,
                std::basic_istream<charT, traits>& is)
{
    auto equal = std::find(line.begin(), line.end(), '=');

    std::basic_string<charT, traits, alloc> key(line.cbegin(), equal);
    key = remove_extraneous_whitespace(key);

    std::basic_string<charT, traits, alloc> value(equal+1, line.cend());
    value = remove_indent(value);

    while(is_multi_line_value(value))
    {
        if(is.eof()) throw end_of_file("eof");
        const auto additional = read_line(is);
        if(additional.empty()) continue;
        value += additional;
    }

    return std::make_pair(key, parse_value(value));
}

template<typename charT, typename traits>
std::shared_ptr<value_base>
parse_table(std::basic_istream<charT, traits>& is)
{
    std::shared_ptr<value_base> retval(new table_type);
    std::shared_ptr<table_type> table =
        std::dynamic_pointer_cast<table_type>(retval);
    while(!is.eof())
    {
        const std::basic_string<charT, traits> line = read_line(is);
        if(line.empty()) continue;
        LineKind kind = determine_line_kind(line);
        switch(kind)
        {
            case LineKind::TABLE_TITLE:
            {
                putback_line(is, line);
                return retval;
            }
            case LineKind::ARRAY_OF_TABLE_TITLE:
            {
                putback_line(is, line);
                return retval;
            }
            case LineKind::KEY_VALUE:
            {
                try
                {
                    const auto kv = parse_key_value(line, is);
                    table->value[kv.first] = kv.second;
                }
                catch(end_of_file& eof){throw syntax_error("sudden eof");}
                break;
            }
            case LineKind::UNKNOWN:
                throw syntax_error("unknown line: " + line); // XXX!
            default:
                throw internal_error("invalid line kind");
        }
    }
    return retval;
}

template<typename charT, typename traits>
Data parse(std::basic_istream<charT, traits>& is)
{
    Data data;
    while(!is.eof())
    {
        const std::basic_string<charT, traits> line = read_line(is);
        if(line.empty()) continue;
        LineKind kind = determine_line_kind(line);
        switch(kind)
        {
            case LineKind::TABLE_TITLE:
            {
                data[extract_table_title(line)] = parse_table(is);
                break;
            }
            case LineKind::ARRAY_OF_TABLE_TITLE:
            {

                break;
            }
            case LineKind::KEY_VALUE:
            {
                try
                {
                    const auto kv = parse_key_value(line, is);
                    data[kv.first] = kv.second;
                }
                catch(end_of_file& eof)
                {
                    throw syntax_error("sudden eof");
                }
                break;
            }
            case LineKind::UNKNOWN:
                throw syntax_error("unknown line: " + line);
            default:
                throw internal_error("invalid line kind");
        }
    }
    return data;
}

}//toml

#endif /* TOML_PARSER */
