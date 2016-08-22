#ifndef TOML_GET_VALUE
#define TOML_GET_VALUE
#include "definitions.hpp"
#include "type_traits.hpp"
#include "toml_values.hpp"
#include <string>
#include <memory>

namespace toml
{
template<typename T> struct get_impl;

template<typename T>
inline T get(const std::shared_ptr<value_base>& val)
{
    static_assert(is_toml_type<T>::value, "use get<T> for toml value");
    return get_impl<T>::apply(val);
}

template<typename T>
struct get_impl
{
    static T apply(const std::shared_ptr<value_base>& val)
    {
        auto tmp = std::dynamic_pointer_cast<typed_value<T>>(val);
        if(!tmp)
            throw type_error<std::string>("not "+std::string(print_type<T>()));
        return tmp->value;
    }
};

template<>
struct get_impl<Table>
{
    static Table apply(const std::shared_ptr<value_base>& val)
    {
        auto tmp = std::dynamic_pointer_cast<table_type>(val);
        if(!tmp) throw type_error<std::string>("not table");
        return tmp->value;
    }
};

template<typename T>
struct get_impl<Array<T>>
{
    static Array<T> apply(const std::shared_ptr<value_base>& val)
    {
        auto tmp = std::dynamic_pointer_cast<array_type>(val);
        if(!tmp) throw type_error<std::string>("not array");
        Array<T> retval; retval.reserve(tmp->value.size());
        for(auto iter = tmp->value.cbegin(); iter != tmp->value.cend(); ++iter)
            retval.push_back(get<T>(*iter));
        return retval;
    }
};

}//toml

#endif /* TOML_GET_VALUE */
