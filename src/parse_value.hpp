#ifndef TOML_PARSE_VALUE
#define TOML_PARSE_VALUE
#include "definitions.hpp"
#include "toml_values.hpp"

namespace toml
{

// to test
template<typename charT, typename traits, typename alloc>
std::shared_ptr<value_base>
parse_value(const std::basic_string<charT, traits, alloc>& str)
{
    return std::make_shared<typed_value<String>>(str);
}

}//toml

#endif /* TOML_PARSE_VALUE */
