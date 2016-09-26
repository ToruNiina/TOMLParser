TOMLParser
====

[![Build Status](https://travis-ci.org/ToruNiina/TOMLParser.svg?branch=travis)](https://travis-ci.org/ToruNiina/TOMLParser)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

C++ header-only TOML Parser.

TOMLParser supports [TOML v0.4.0](http://github.com/toml-lang/toml/blob/master/README.md).

If you can use c++11 features, this parser depends only on STL.
If not so, this parser depends on Boost C++ library.

To use this with C++98 & Boost, the only thing you have to do is installing boost.

## Usage

Because this library is header-only, you can use this parser without build.

You can easily see how to use this library if you read the following code.

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

With c++11, you can read above file using the code described below.

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
        toml::get<toml::Array<toml::Array<bool>>>(table.at("nested_array"));
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
    int foobar0 = toml::get<toml::Integer>(array_of_table.at(0).at("foobar"));
    int foobar1 = toml::get<toml::Integer>(array_of_table.at(1).at("foobar"));
    int foobar2 = toml::get<toml::Integer>(array_of_table.at(2).at("foobar"));

    return 0;
}
```

function ```toml::get<T>``` is useful, but you cannot use straightforwardly
this function for the value like this.
```toml
    array_of_array = [[1.0, 2.0, 3.0], ["a", "b", "c"]]
```
In this case, ```toml::ValueBase``` will help you.
```cpp
    std::vector<toml::ValueBase> array_of_array =
        toml::get<toml::Array<toml::ValueBase>>(data.at("array_of_array"));
    std::vector<double> first_array =
        toml::get<toml::Array<toml::Float>>(array_of_array.at(0));
    std::vector<std::string> second_array =
        toml::get<toml::Array<toml::String>>(array_of_array.at(1));
```

In C++98, some of the types and methods are different.
For details, see Documentation.

## Documentation

In this parser, all the TOML types are subclass of ```toml::value_base```.
```toml::ValueBase``` is just a typedef of ```toml::shared_ptr<toml::value_base>```.

Supported TOML types are listed below.

| TOML type | type names in this parser | actual type in c++11 case         |
|:----------|:---------------------|:---------------------------------------|
| Boolean   | toml::Boolean        | bool                                   |
| Integer   | toml::Integer        | std::int\_least64\_t                   |
| Float     | toml::Float          | double                                 |
| String    | toml::String         | std::string                            |
| Datetime  | toml::Datetime       | std::chrono::system\_clock::time\_point|
| Array     | toml::Array< typename T > | std::vector < T >                 |
| Table     | toml::Table          | std::map< std::string, std::shared\_ptr < toml::value\_base > > |

Also, ```toml::Data``` is just a typedef of ```toml::Table```.

In C++98, ```toml::Array<T>``` become a type generator struct because there are
no template using. And there are no ```at``` method in ```std::map```, so you 
should use ```operator[]``` to access the values.

Additionally, there are no ```std::chrono```, ```std::int_least64_t``` and
```std::shared_ptr```, so ```boost::chrono```  ```boost::int\_least64\_t``` and 
```boost::shared_ptr``` are used instead.

| TOML type | type names in this parser | actual type in c++98 case         |
|:----------|:---------------------|:---------------------------------------|
| Boolean   | toml::Boolean        | bool                                   |
| Integer   | toml::Integer        | boost::int\_least64\_t                 |
| Float     | toml::Float          | double                                 |
| String    | toml::String         | std::string                            |
| Datetime  | toml::Datetime       | boost::chrono::system\_clock::time\_point|
| Array     | toml::Array< typename T >::type | std::vector < T >                 |
| Table     | toml::Table          | std::map< std::string, boost::shared\_ptr < toml::value\_base > > |

So, the different part of the code in C++98 is below.

```cpp

    boost::int_least64_t number = toml::get<toml::Integer>(table.at("number");
    boost::chrono::system_clock::time_point date =
        toml::get<toml::Datetime>(table.at("date"));

    std::vector<double> reals =
        toml::get<toml::Array<toml::Float>::type>(table.at("reals"));

//  If you do not want to write "::type", you can use std::vector as toml::Array.
//  std::vector<double> reals =
//      toml::get<std::vector<toml::Float>>(table.at("reals"));
```

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
