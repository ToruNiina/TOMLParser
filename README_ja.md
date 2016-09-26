TOMLParser
====

[![Build Status](https://travis-ci.org/ToruNiina/TOMLParser.svg?branch=travis)](https://travis-ci.org/ToruNiina/TOMLParser)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

C++用のheader-onlyなTOML parserです。

[TOML v0.4.0](http://github.com/toml-lang/toml/blob/master/README.md)に対応しています。

C++11が使える場合、標準ライブラリにしか依存しません。
そうでなければ、Boost C++ Libraryに依存します。
ただし、Boostを用いる場合も、インストールの他にするべきことはありません。

## Usage

header-onlyなので、インクルードパスを通し、インクルードするだけで使えるようになります。
ビルドの必要はありません。

使い方は非常に単純です。

以下の様なTOMLファイルがあるとき、

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

以下のコードによってファイル内のデータを取得できます(c++11の場合)。

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

```toml::get<T>```関数は便利ですが、特性上以下のような型にはそのままでは使えません。
```toml
    array_of_array = [[1.0, 2.0, 3.0], ["a", "b", "c"]]
```

この場合、toml::ValueBaseを用います。
```cpp
    std::vector<toml::ValueBase> array_of_array =
        toml::get<toml::Array<toml::ValueBase>>(data.at("array_of_array"));
    std::vector<double> first_array =
        toml::get<toml::Array<toml::Float>>(array_of_array.at(0));
    std::vector<std::string> second_array =
        toml::get<toml::Array<toml::String>>(array_of_array.at(1));
```

C++98を用いる場合、いくつかの型名や仕様できるメソッドが異なります。
詳細はDocumentationをご覧ください。

より詳細な例として、sample/example.cppにtoml-lang/tomlのexample-v0.4.0.tomlを
読むコードがあります。

## Documentation

このライブラリでは、全てのTOML型は```toml::value_base```から派生しています。
上記の```toml::ValueBase```は、```toml::shared_ptr<toml::value_base>```のエイリアスです。

TOMLの型名は以下の様になっています。

| TOML型    | このパーサ内での型名 | c++11での実際のデータ型                |
|:----------|:---------------------|:---------------------------------------|
| Boolean   | toml::Boolean        | bool                                   |
| Integer   | toml::Integer        | std::int\_least64\_t                   |
| Float     | toml::Float          | double                                 |
| String    | toml::String         | std::string                            |
| Datetime  | toml::Datetime       | std::chrono::system\_clock::time\_point|
| Array     | toml::Array< typename T > | std::vector < T >                 |
| Table     | toml::Table          | std::map< std::string, std::shared\_ptr < toml::value\_base > > |

サンプルコードにあった```toml::Data```は，実際には単なる```toml::Table```の```typedef```です。

C++98の場合、template usingが使えないため```toml::Array<T>```はtype generatorとなります。
さらに、```std::map```に```at```メソッドが存在しないため、```toml::Table```の
要素アクセスには```operator[]```を用いる必要があります。

また，```std::chrono```や```std::int_least64_t```や```std::shared_ptr```が存在
しないため、これらは```boost::chrono```，```boost::int\_least64\_t```，
そして```boost::shared_ptr```に置き換わります。

| TOML型    | このパーサ内での型名 | c++98での実際のデータ型                |
|:----------|:---------------------|:---------------------------------------|
| Boolean   | toml::Boolean        | bool                                   |
| Integer   | toml::Integer        | boost::int\_least64\_t                 |
| Float     | toml::Float          | double                                 |
| String    | toml::String         | std::string                            |
| Datetime  | toml::Datetime       | boost::chrono::system\_clock::time\_point|
| Array     | toml::Array< typename T >::type | std::vector < T >                 |
| Table     | toml::Table          | std::map< std::string, boost::shared\_ptr < toml::value\_base > > |

よって、C++98でのコードで異なる部分は、以下の様になります。

```cpp

    boost::int_least64_t number = toml::get<toml::Integer>(table.at("number");
    boost::chrono::system_clock::time_point date =
        toml::get<toml::Datetime>(table.at("date"));

    std::vector<double> reals =
        toml::get<toml::Array<toml::Float>::type>(table.at("reals"));

//  ::typeを書くのが面倒なら、std::vectorをtoml::Arrayとして使うこともできます
//  std::vector<double> reals =
//      toml::get<std::vector<toml::Float>>(table.at("reals"));
```

## Testing

Boost.unittestフレームワークと、CMake CTestを用います。
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
