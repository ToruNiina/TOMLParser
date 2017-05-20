#ifndef TOML_GET_VALUE
#define TOML_GET_VALUE
#include "definitions.hpp"
#include "type_traits.hpp"
#include "toml_values.hpp"

namespace toml
{
template<typename T> struct get_impl;

template<typename T>
T get(const toml::shared_ptr<value_base>& val)
{
    return get_impl<T>::invoke(val);
}

template<typename T>
struct get_impl
{
    static T invoke(const toml::shared_ptr<value_base>& val)
    {
        toml::shared_ptr<typed_value<T> > tmp =
            toml::dynamic_pointer_cast<typed_value<T> >(val);
        if(!tmp) throw type_error("not " + std::string(as_string<T>()));
        return tmp->value;
    }
};

template<>
struct get_impl<Table>
{
    static Table invoke(const toml::shared_ptr<value_base>& val)
    {
        shared_ptr<table_type<map_key_charactor_helper<Table>::char_type> >
            tmp = toml::dynamic_pointer_cast<
                table_type<map_key_charactor_helper<Table>::char_type> >(val);
        if(!tmp) throw type_error("not table type");
        return tmp->value;
    }
};

#ifdef TOML_ENABLE_CXX11
template<typename T>
struct get_impl<Array<T>>
{
    static Array<T> invoke(const toml::shared_ptr<value_base>& val)
    {
        auto tmp = toml::dynamic_pointer_cast<array_type>(val);
        if(!tmp) throw type_error("not array type");

        Array<T> retval;
        retval.reserve(tmp->value.size());

        for(auto iter = tmp->value.cbegin(); iter != tmp->value.cend(); ++iter)
            retval.push_back(get<T>(*iter));

        return retval;
    }
};

template<>
struct get_impl<Array<shared_ptr<value_base>>>
{
    static Array<shared_ptr<value_base>>
    invoke(const toml::shared_ptr<value_base>& val)
    {
        auto tmp = toml::dynamic_pointer_cast<array_type>(val);
        if(!tmp) throw type_error("not array type");
        return tmp->value;
    }
};

#else // c++98
template<typename T>
struct get_impl<std::vector<T> >
{
    static std::vector<T> invoke(const toml::shared_ptr<value_base>& val)
    {
        toml::shared_ptr<array_type> tmp =
            toml::dynamic_pointer_cast<array_type>(val);
        if(!tmp) throw type_error("not array type");

        std::vector<T> retval;
        retval.reserve(tmp->value.size());

        for(typename std::vector<toml::shared_ptr<value_base> >::const_iterator
                iter = tmp->value.begin(); iter != tmp->value.end(); ++iter)
            retval.push_back(get<T>(*iter));

        return retval;
    }
};

template<>
struct get_impl<std::vector<shared_ptr<value_base> > >
{
    static std::vector<shared_ptr<value_base> >
    invoke(const toml::shared_ptr<value_base>& val)
    {
        toml::shared_ptr<array_type> tmp =
            toml::dynamic_pointer_cast<array_type>(val);
        if(!tmp) throw type_error("not array type");
        return tmp->value;
    }
};
#endif

}//toml

#endif /* TOML_GET_VALUE */
