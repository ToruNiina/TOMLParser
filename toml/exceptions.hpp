#ifndef TOML_EXCEPTIONS
#define TOML_EXCEPTIONS
#include "cxxconfig.hpp"
#include <stdexcept>
#include <string>

namespace toml
{

class exception : public std::exception
{
  public:
    exception() TOML_NOEXCEPT {}
    virtual ~exception() TOML_NOEXCEPT TOML_OVERRIDE {}

    virtual const char* what() const TOML_NOEXCEPT TOML_OVERRIDE {return "";}
};

class syntax_error : public exception
{
  public:
    syntax_error(const std::string& str) : str_(str){}
    virtual ~syntax_error() TOML_NOEXCEPT TOML_OVERRIDE {}

    virtual const char* what() const TOML_NOEXCEPT TOML_OVERRIDE
    {
        return str_.c_str();
    }

  private:
    std::string str_;
};

class type_error : public exception
{
  public:
    type_error(const std::string& str) : str_(str){}
    virtual ~type_error() TOML_NOEXCEPT TOML_OVERRIDE {}

    virtual const char* what() const TOML_NOEXCEPT TOML_OVERRIDE
    {
        return str_.c_str();
    }

  private:
    std::string str_;
};

class internal_error : public exception
{
  public:
    internal_error(const std::string& str) : str_(str){}
    virtual ~internal_error() TOML_NOEXCEPT TOML_OVERRIDE {}

    virtual const char* what() const TOML_NOEXCEPT TOML_OVERRIDE
    {
        return str_.c_str();
    }

  private:
    std::string str_;
};
}//toml
#endif /* TOML_EXCEPTIONS */
