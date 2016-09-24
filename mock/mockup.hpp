#include <memory>
#include <vector>
#include <string>
#include <map>
#include <iostream>

enum class Types
{
    String,
    Array,
    Table,
};

struct value_base
{
    Types type;

    value_base() = default;
    value_base(Types t) : type(t) {}
    virtual ~value_base() noexcept = default;
};

struct string_value : public value_base
{
    std::string value;

    string_value(): value_base(Types::String){};
    string_value(const std::string& str): value_base(Types::String), value(str){};
    ~string_value() noexcept override = default;
};

struct array_value : public value_base
{
    std::vector<std::shared_ptr<value_base>> value;

    array_value(): value_base(Types::Array){};
    ~array_value() noexcept override = default;
};

struct table_value : public value_base
{
    std::map<std::string, std::shared_ptr<value_base>> value;
    table_value() : value_base(Types::Table){}
    ~table_value() noexcept override = default;
};

std::shared_ptr<value_base>
make_nested_table(
        typename std::vector<std::string>::const_iterator iter,
        typename std::vector<std::string>::const_iterator end,
        std::shared_ptr<value_base> contents, bool is_array)
{
    if(iter == end)
    {
        if(is_array)
        {
            auto ar = std::make_shared<array_value>();
            ar->value.push_back(contents);
            return ar;
        }
        return contents;
    }
    auto tmp = std::make_shared<table_value>();
    tmp->value[*iter] = make_nested_table(iter + 1, end, contents, is_array);
    return tmp;
}

void
search_and_make_table(std::shared_ptr<value_base> root,
        typename std::vector<std::string>::const_iterator iter,
        typename std::vector<std::string>::const_iterator end,
        std::shared_ptr<value_base> contents, bool is_array)
{
    if(iter == end)
    {
        auto arr = std::dynamic_pointer_cast<array_value>(root);
        if(arr)
        {
            if(!is_array) std::logic_error("array of table definition incorrect");
            arr->value.push_back(contents);
            return;
        }
        throw std::logic_error("array of table name conflicts");
    }
    else
    {
        std::shared_ptr<table_value> tab;

        auto arr = std::dynamic_pointer_cast<array_value>(root);
        if(arr)
            tab = std::dynamic_pointer_cast<table_value>(arr->value.back());
        else
            tab = std::dynamic_pointer_cast<table_value>(root);

        if(tab->value.count(*iter) == 1)
            search_and_make_table(tab->value[*iter], iter+1, end, contents, is_array);
        else
            tab->value[*iter] = make_nested_table(iter+1, end, contents, is_array);
        return;
    }
}
