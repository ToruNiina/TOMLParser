#ifndef TOML_VALUES
#define TOML_VALUES
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace toml
{

template<typename T>
struct typed_value : public value_base
{
    T value;
    typed_value() = default;
    ~typed_value() noexcept override = default;
};

struct array_type : public value_base
{
    std::vector<std::shared_ptr<value_base>> value;
    ~array_type() noexcept override = default;
};

struct table_type : public value_base
{
    std::map<std::string, std::shared_ptr<value_base>> value;
    ~table_type() noexcept override = default;
};

}//toml

#endif /* TOML_VALUES */
