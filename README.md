TOMLParser
====

C++ header-only TOML Parser.

development is now ongoing. some features of TOML listed below is not supported.
- quated keys
- nested table with Dots
- escape sequence in basic string
- include extraneous whitespace before '\' in multi-line basic string

## Usage

You can easily see how to use this library if you read the following code.

    #include "toml.hpp"
    #include <fstream>

    std::ifstream file("sample.toml");
    toml::Data                      data  = toml::parse(file);
    std::string                     title = toml::get<toml::String>(data.at("title"));
    toml::Table                     table = toml::get<toml::Table>(data.at("table"));
    std::vector<toml::Table>        array = toml::get<toml::Array<toml::Table>>(data.at("array_of_table"));
    
    std::int_least64_t              foo   = toml::get<toml::Integer>(table.at("foo"));
    std::vector<double>             bar   = toml::get<toml::Array<Float>>(table.at("bar"));
    std::vector<std::vector<bool>>  baz   = toml::get<toml::Array<toml::Array<Boolean>>>(table.at("baz"));
    toml::Table                     qux   = toml::get<toml::Table>(table.at("qux"));
    std::string                     quux  = toml::get<toml::String>(qux.at("quux")); 

you can parse the following toml file with this code.

    # sample.toml
    title = "this is sample"

    [table]
    foo = +100_000
    bar = [-1.1e+2, 2.0, 3.0]
    baz = [[true, false], # this is a comment.
           [true], [false]]
    qux = {quux = "inline table"}

    [[array_of_table]]
    foobar = 1
    [[array_of_table]]
    foobar = 2
    [[array_of_table]]
    foobar = 3

Supported TOML types are listed below.
All the supported toml-type are in the namespace "toml".

| TOML type | typename in this library| you can get data as                    |
|:----------|:------------------------|:---------------------------------------|
| Boolean   | toml::Boolean           | bool                                   |
| Integer   | toml::Integer           | std::int\_least64\_t                   |
| Float     | toml::Float             | double                                 |
| String    | toml::String            | std::string                            |
| Datetime  | toml::Datetime          | std::chrono::system\_clock::time\_point|
| Array     | toml::Array< typename T > | std::vector < T >                    |
| Table     | toml::Table             | std::map< std::string, std::shared\_ptr < value\_base > > |

toml::Data type is same as toml::Table. there are no difference.

value\_base is empty struct and is superclass of all other (library-internal)
toml value types. You can get the value from shared pointer to value\_base using

    template<typename T>
    T toml::get<T>(std::shared_ptr<value_base> const&)

function.

to see more detailed usage, please see sample/sample.cpp and sample/sample.toml.

## Dependency

Core codes depend on c++11 standard library only.

## Install

This library is header-only library, so the only thing you have to do is add
path/to/this/library to include path.

## Documents

sorry, not written yet.

## Testing

sorry, not implemented yet.

## Licensing terms

This project is licensed under the terms of the MIT License.
See LICENSE for the project license.

- Copyright (c) 2016 Toru Niina

All rights reserved.
