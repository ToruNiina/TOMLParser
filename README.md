TOMLParser
====

C++ header-only TOML Parser.

developping is now ongoing. following is just a goal.

<!--
 ## Usage

 You can easily see how to use this library with reading following code.

     #include "toml.hpp"
     #include <fstream>
     std::ifstream file("filename.toml");
     toml::Data          data  = toml::parse(file);
     toml::Table         table = data.at("tablename");
     bool                value = get<toml::Boolean>(table.at("foo"));
     std::vector<double> array = get<toml::Array<Floating>>(table.at("bar"));
     std::vector<toml::Table> array_of_table = data.at("arrayOfTable");

 this code can parse the following toml file.

     # filename.toml
     [tablename]
     foo = true
     bar = [1.0, 2.0, 3.0]

     [[arrayOfTable]]
     hoge = "piyo"
     [[arrayOfTable]]
     hoge = "fuga"
     # or
     # arrayOfTable = [
     #     {hoge = "piyo"}
     #     {hoge = "fuga"}
     # ]

 Supported TOML types are listed below. All the supported toml-type are in the namespace "toml".

 | toml type | typename in this library| you can get data as |
 |:----------|:------------------------|:------------|
 | Boolean   | Boolean                 | bool        |
 | Integer   | Integer                 | int64\_t    |
 | Float     | Float                   | double      |
 | String    | String                  | std::string |
 | Datetime  | Datetime                | std::chrono::system\_clock::time\_point|
 | Array     | Array<typename T_value> | std::vector<value_base>           |
 | Table     | Table                   | std::map<std::string, value_base> |

 ### Dependency

 Core codes depend on c++11 standard library only.

 ## Install 

 This library is header-only library, so the only thing you have to do is add
 path/to/this/library to include path.

 ## Documents

 Depends on Doxygen and CMake.
 To generate document of this library, run following commands.

     cd build
     cmake ..
     make doc

 ## Testing

 Test codes depends on Boost unittest framework and CMake CTest.
 To test the code, run following commands.

     cd build
     cmake ..
     make test
-->

## Licensing terms

This project is licensed under the terms of the MIT License.
See LICENSE for the project license.

- Copyright (c) 2016 Toru Niina

All rights reserved.
