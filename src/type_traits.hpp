#ifndef TOML_TYPE_TRAITS
#define TOML_TYPE_TRAITS
#include "toml_values.hpp"

namespace toml
{

template<typename T>
struct map_key_charactor_helper{};

template<typename charT, typename valueT>
struct map_key_charactor_helper<std::map<std::basic_string<charT>, valueT> >
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
