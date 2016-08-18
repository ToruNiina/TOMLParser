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
struct is_toml_type<Boolean>  : public std::true_type{};
template<>
struct is_toml_type<Integer>  : public std::true_type{};
template<>
struct is_toml_type<Float>    : public std::true_type{};
template<>
struct is_toml_type<String>   : public std::true_type{};
template<>
struct is_toml_type<Datetime> : public std::true_type{};
template<typename T>
struct is_toml_type<Array<T>> : public std::true_type{};
template<>                    
struct is_toml_type<Table>    : public std::true_type{};

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
template<>
struct is_toml_struct<table_type> : public std::true_type{};

template<typename T>
inline const char* print_type();
template<>
inline const char* print_type<Boolean>(){return "Boolean";}
template<>
inline const char* print_type<Integer>(){return "Integer";}
template<>
inline const char* print_type<Float>(){return "Float";}
template<>
inline const char* print_type<String>(){return "String";}
template<>
inline const char* print_type<Datetime>(){return "Datetime";}
template<>
inline const char* print_type<array_type>(){return "array_type";}
template<>
inline const char* print_type<table_type>(){return "table_type";}

}//toml

#endif /* TOML_TYPE_TRAITS */
