#ifndef TOML_DEFINITIONS
#define TOML_DEFINITIONS
#include "cxxconfig.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <cstdint>

namespace toml
{

struct value_base{virtual ~value_base() TOML_NOEXCEPT {}};

typedef std::string                      String;
typedef int_least64_t                    Integer;
typedef double                           Float;
typedef bool                             Boolean;
typedef toml::chrono::system_clock::time_point Datetime;
typedef std::map<std::string, shared_ptr<value_base> > Table;
typedef Table Data;

#ifdef TOML_ENABLE_CXX11
    template<typename T> using Array = std::vector<T>;
#else
    template<typename T> struct Array{typedef std::vector<T> type;};
#endif

}

#endif /* TOML_DEFINITIONS */
