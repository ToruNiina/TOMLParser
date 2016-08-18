#ifndef TOML_DEFINITIONS
#define TOML_DEFINITIONS
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <cstdint>

namespace toml
{

// empty class. all other toml value classes are subclass of this.
struct value_base{virtual ~value_base() noexcept = default;};

using String   = std::string;
using Integer  = std::int_least64_t;
using Float    = double;
using Boolean  = bool;
using Datetime = std::chrono::system_clock::time_point;
template<typename T>
using Array = std::vector<T>;
using Table = std::map<std::string, std::shared_ptr<value_base>>;
using Data  = Table;

}

#endif /* TOML_DEFINITIONS */
