#ifndef TOML_EXCEPTIONS
#define TOML_EXCEPTIONS
#include <stdexcept>
#include <string>

namespace toml
{

class exception : public std::exception
{
  public:
    exception() noexcept = default;
    virtual ~exception() noexcept override = default;

    virtual const char* what() const noexcept override {return "";}
};

class file_error : public exception
{
  public:
    file_error(const std::string& str) : str_(str) {}
    virtual ~file_error() noexcept override = default;

    virtual const char* what() const noexcept override
    {
        return str_.c_str();
    }

  private:
    std::string str_;
};

class end_of_file : public file_error
{
  public:
    end_of_file(const std::string& str) : file_error(str){}
    virtual ~end_of_file() noexcept override = default;
};

template<typename charT>
class syntax_error : public exception
{
  public:
    syntax_error(const std::basic_string<charT>& str):str_(str){}
    virtual ~syntax_error() noexcept override = default;

    virtual const char* what() const noexcept override
    {
        return str_.c_str();
    }

  private:
    std::basic_string<charT> str_;
};

template<typename charT>
class type_error : public exception
{
  public:
    type_error(const std::basic_string<charT>& str):str_(str){}
    virtual ~type_error() noexcept override = default;

    virtual const char* what() const noexcept override
    {
        return str_.c_str();
    }

  private:
    std::basic_string<charT> str_;
};

template<typename charT>
class internal_error : public exception
{
  public:
    internal_error(const std::basic_string<charT>& str):str_(str){}
    virtual ~internal_error() noexcept override = default;

    virtual const char* what() const noexcept override
    {
        return str_.c_str();
    }

  private:
    std::basic_string<charT> str_;
};


}
#endif /* TOML_EXCEPTIONS */
