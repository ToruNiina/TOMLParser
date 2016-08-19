TOMLParser
====

C++ header-only TOML Parser.

## Usage

You can easily see how to use this library with reading following code.

    #include "toml.hpp"
    #include <fstream>
    std::ifstream file("filename.toml");
    toml::Data                      data  = toml::parse(file);
    std::string                     title = toml::get<toml::String>(data.at("title"));
    toml::Table                     table = toml::get<toml::Table>(data.at("table"));
    bool                            foo   = toml::get<toml::Boolean>(table.at("foo"));
    std::vector<double>             bar   = toml::get<toml::Array<Floating>>(table.at("bar"));
    std::vector<std::int_least64_t> baz   = toml::get<toml::Array<Floating>>(table.at("baz"));

this code can parse the following toml file.

    # filename.toml
    title = "this is sample"
    [table]
    foo = true
    bar = [1.0, 2.0, 3.0]
    baz = [100_000, 200_000, 300_000_000]

Supported TOML types are listed below.
All the supported toml-type are in the namespace "toml".

| toml type | typename in this library| you can get data as                    |
|:----------|:------------------------|:---------------------------------------|
| Boolean   | toml::Boolean           | bool                                   |
| Integer   | toml::Integer           | std::int\_least64\_t                   |
| Float     | toml::Float             | double                                 |
| String    | toml::String            | std::string                            |
| Datetime  | toml::Datetime          | std::chrono::system\_clock::time\_point|
| Array     | toml::Array<typename T> | std::vector<T>                         |
| Table     | toml::Table             | std::map<std::string, value_base>      |

toml::Data type is same as toml::Table. there are no difference.

### Dependency

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
