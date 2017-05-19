TOMLParser
====

[![Build Status](https://travis-ci.org/ToruNiina/TOMLParser.svg?branch=travis)](https://travis-ci.org/ToruNiina/TOMLParser)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

C++ header-only TOML Parser.

__NOTE__: New c++11 toml library named [toml11](https://github.com/ToruNiina/toml11) is ready. If you can use c++11, use it. From now, this repository focus on c++98 & boost. In the next release, I plan to change some of the interfaces.

__TOMLParser__ supports [TOML v0.4.0](http://github.com/toml-lang/toml/blob/master/README.md).

If you can use c++11 features, this parser depends only on the STL.
If not so, it depends on the Boost C++ library.

To use this parser with c++98 & boost, it is better that download the boost library
from the official site.

[日本語版README](README_ja.md)

## Usage

Because this is header-only library, you can use __TOMLParser__ without build.

You can easily see how to use it if you read the following sample codes.

To parse a TOML-file, use the `toml::Data toml::parse(std::basic_istream<charT>&)` function.

```cpp
std::ifstream ifs("sample.toml");
toml::Data data = toml::parse(ifs);
```

You can get a TOML-value using the `T toml::get<T>` function.

```toml
# toml file
title = "title"
```

```cpp
std::string title = toml::get<toml::String>(data.at("title")); // "title"
```

You can also use the `T toml::get<T>` function for an array.

```toml
# toml file
array = [3, 3.1, 3.14, 3.141, 3.1415]
```

```cpp
// {3, 3.1, 3.14, 3.141, 3.1415}
std::vector<double> array = toml::get<toml::Array<toml::Float>>(data.at("array"));
```

If you have a TOML-file like the following,

```toml
# sample.toml
title = "this is sample"

[table]
number = +100_000
reals  = [-1.1e+2, 2.0, 3.0]
nested_array = [[true, false], # this is a comment.
                [true],
                [false]]
date = 1979-05-27
inline_table = {name = "inline table"}
array_of_inline_table = [
    {key = "array"},
    {key = "of"},
    {key = "inline"},
    {key = "table"},
    ]

[[array_of_table]]
foobar = 1
[[array_of_table]]
foobar = 2
[[array_of_table]]
foobar = 3
```

you can read and parse it using the c++11 code described below.

```cpp
#include "toml.hpp"
#include <fstream>

int main()
{
    std::ifstream file("sample.toml");
    if(not file.good()) return 1;

    toml::Data data = toml::parse(file);
    std::string title = toml::get<toml::String>(data.at("title"));
    toml::Table table = toml::get<toml::Table>(data.at("table"));
    std::int_least64_t number = toml::get<toml::Integer>(table.at("number");
    std::vector<double> reals =
        toml::get<toml::Array<toml::Float>>(table.at("reals"));
    std::vector<std::vector<bool>> nested_array =
        toml::get<toml::Array<toml::Array<toml::Boolean>>>(table.at("nested_array"));
    std::chrono::system_clock::time_point date =
        toml::get<toml::Datetime>(table.at("date"));
    toml::Table inline_table = toml::get<toml::Table>(table.at("inline_table"));
    std::string name = toml::get<toml::String>(inline_table.at("name"));

    std::vector<toml::Table> array_of_inline_table = 
        toml::get<toml::Array<toml::Table>>(table.at("array_of_inline_table"));
    std::string key0 = toml::get<toml::String>(array_of_inline_table.at(0).at("key"));
    std::string key1 = toml::get<toml::String>(array_of_inline_table.at(1).at("key"));
    std::string key2 = toml::get<toml::String>(array_of_inline_table.at(2).at("key"));
    std::string key3 = toml::get<toml::String>(array_of_inline_table.at(3).at("key"));

    std::vector<toml::Table> array_of_table = 
        toml::get<toml::Array<toml::Table>>(table.at("array_of_table"));
    std::int_least64_t foobar0 = toml::get<toml::Integer>(array_of_table.at(0).at("foobar"));
    std::int_least64_t foobar1 = toml::get<toml::Integer>(array_of_table.at(1).at("foobar"));
    std::int_least64_t foobar2 = toml::get<toml::Integer>(array_of_table.at(2).at("foobar"));

    return 0;
}
```

Even though the function `toml::get<T>` is useful,
you cannot use it straightforwardly for a value like this.

```toml
array_of_array = [[1.0, 2.0, 3.0], ["a", "b", "c"]]
```

In this case, `toml::ValueBase` will help you.

```cpp
std::vector<toml::ValueBase> array_of_array =
    toml::get<toml::Array<toml::ValueBase>>(data.at("array_of_array"));
std::vector<double> first_array =
    toml::get<toml::Array<toml::Float>>(array_of_array.at(0));
std::vector<std::string> second_array =
    toml::get<toml::Array<toml::String>>(array_of_array.at(1));
```

In C++98, some of the types and methods are different.
For more detail, see Documentation.

## Documentation

In this parser, all the TOML types are the subclass of `toml::value_base`.
Actually, `toml::ValueBase` is just a `typedef` of `toml::shared_ptr<toml::value_base>`.

All the supported TOML types are listed below.

| TOML type | type names in this parser | actual type in c++11                   |
|:----------|:--------------------------|:---------------------------------------|
| Boolean   | `toml::Boolean`           | `bool`                                 |
| Integer   | `toml::Integer`           | `std::int_least64_t`                   |
| Float     | `toml::Float`             | `double`                               |
| String    | `toml::String`            | `std::string`                          |
| Datetime  | `toml::Datetime`          | `std::chrono::system_clock::time_point`|
| Array     | `toml::Array<typename T>` | `std::vector<T>`                       |
| Table     | `toml::Table`             | `std::map<std::string, std::shared_ptr<toml::value_base>>` |

Also, `toml::Data` is just a typedef of `toml::Table`.

In C++98, `toml::Array<T>` become a struct that is a `type generator`
because there are no `template using alias`.
And there are no `at` method in `std::map`,
so you should use `operator[]` to access the values.

Additionally, there are no `std::chrono`, `std::int_least64_t`
and `std::shared_ptr`, so `boost::chrono`, `boost::int_least64_t`
and `boost::shared_ptr` are used instead.

| TOML type | type names in this parser | actual type in c++98                     |
|:----------|:--------------------------|:-----------------------------------------|
| Boolean   | `toml::Boolean`           | `bool`                                   |
| Integer   | `toml::Integer`           | `boost::int_least64_t`                   |
| Float     | `toml::Float`             | `double`                                 |
| String    | `toml::String`            | `std::string`                            |
| Datetime  | `toml::Datetime`          | `boost::chrono::system_clock::time_point`|
| Array     | `toml::Array<typename T>::type` | `std::vector<T>`                   |
| Table     | `toml::Table`             | `std::map<std::string, boost::shared_ptr<toml::value_base>>` |

So, the different part of the sample code in C++98 is below.

```cpp

boost::int_least64_t number = toml::get<toml::Integer>(table.at("number");
boost::chrono::system_clock::time_point date =
    toml::get<toml::Datetime>(table.at("date"));

std::vector<double> reals =
    toml::get<toml::Array<toml::Float>::type>(table.at("reals"));

//  If you do not want to write "::type", you can use std::vector instead of toml::Array.
//  std::vector<double> reals =
//      toml::get<std::vector<toml::Float>>(table.at("reals"));
```

__Note__: When you install boost using apt or some other package manager
that provides binaries already built, some source files possibly does not exist.
If you fail to build your own project that includes this parser using boost and c++98
with an error message like "boost/../libs/something not found", please download
boost library from the official site.
The reason of this problem is you and this parser are forcing to use
```boost/chrono``` as header-only.

### Excpetions

If the syntax is invalid or some sort of an internal error occurs,
exception ```toml::syntax_error``` or ```toml::internal_error``` is thrown by
the function ```toml::parse```, respectively.

If an invalid type is set as a template parameter of the ```toml::get<T>``` function, 
exception ```toml::type_error``` is thrown.

All the exception classes are the subclass of ```toml::exception``` which is
the subclass of ```std::exception```.

## Testing

Using Boost Unit Test Framework and CMake CTest.

```
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ make test
```

## Licensing terms

This project is licensed under the terms of the MIT License.
See LICENSE for the project license.

- Copyright (c) 2016 Toru Niina

All rights reserved.
