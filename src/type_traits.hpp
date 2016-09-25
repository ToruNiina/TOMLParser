#ifndef TOML_TYPE_TRAITS
#define TOML_TYPE_TRAITS
#include "definitions.hpp"
#include "toml_values.hpp"
#include <type_traits>

namespace toml
{

template<typename T>
struct is_toml_type : public std::false_type{};
template<>
struct is_toml_type<Boolean>   : public std::true_type{};
template<>
struct is_toml_type<Integer>   : public std::true_type{};
template<>
struct is_toml_type<Float>     : public std::true_type{};
template<>
struct is_toml_type<String>    : public std::true_type{};
template<>
struct is_toml_type<Datetime>  : public std::true_type{};
template<typename T>
struct is_toml_type<Array<T> > : public std::true_type{};
template<>                    
struct is_toml_type<Table>     : public std::true_type{};

#ifndef TOML_ENABLE_CXX11
// in c++98 case, use std::vector as Array for simplicity.
template<typename T>
struct is_toml_type<std::vector<T> > : public std::true_type{};
#endif

template<typename T>
struct is_toml_simple_type : public std::false_type{};
template<>
struct is_toml_simple_type<Boolean>  : public std::true_type{};
template<>
struct is_toml_simple_type<Integer>  : public std::true_type{};
template<>
struct is_toml_simple_type<Float>    : public std::true_type{};
template<>
struct is_toml_simple_type<String>   : public std::true_type{};
template<>
struct is_toml_simple_type<Datetime> : public std::true_type{};

template<typename T>
struct is_toml_struct : public std::false_type{};
template<>
struct is_toml_struct<array_type> : public std::true_type{};
template<typename charT>
struct is_toml_struct<table_type<charT> > : public std::true_type{};

template<typename T>
struct map_key_charactor_helper{};
template<typename charT>
struct map_key_charactor_helper<
    std::map<std::basic_string<charT>, shared_ptr<value_base> > >
{
    typedef charT char_type;
};

template<typename T>
struct as_string_impl;

template<>
struct as_string_impl<Boolean>{static const char* invoke(){return "Boolean";}};
template<>
struct as_string_impl<Integer>{static const char* invoke(){return "Integer";}};
template<>
struct as_string_impl<Float>{static const char* invoke(){return "Float";}};
template<>
struct as_string_impl<String>{static const char* invoke(){return "String";}};
template<>
struct as_string_impl<Datetime>{static const char* invoke(){return "Datetime";}};
template<>
struct as_string_impl<array_type>{static const char* invoke(){return "array_type";}};
template<typename charT>
struct as_string_impl<table_type<charT> >{static const char* invoke(){return "table_type";}};

template<typename T>
inline const char* as_string()
{
    return as_string_impl<T>::invoke();
}

}//toml

#endif /* TOML_TYPE_TRAITS */
