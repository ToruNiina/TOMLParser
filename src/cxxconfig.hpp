#ifndef TOML_CXX_CONFIG
#define TOML_CXX_CONFIG

#ifndef __cplusplus
#error "__cplusplus macro is not defined."
#endif

#if __cplusplus >= 201103L
#   define TOML_ENABLE_CXX11 1
#   define TOML_CONSTEXPR constexpr
#   define TOML_NOEXCEPT  noexcept
#   define TOML_OVERRIDE  override
#   define TOML_NULLPTR   nullptr
#else
#   define TOML_CONSTEXPR
#   define TOML_NOEXCEPT  throw()
#   define TOML_OVERRIDE
#   define TOML_NULLPTR   NULL
#endif

#ifdef TOML_ENABLE_CXX11
#   include <memory>
#   include <chrono>
#   include <cstdint>
    namespace toml
    {
        using std::shared_ptr;
        using std::make_shared;
        using std::dynamic_pointer_cast;
        using std::chrono;
        using std::int_least64_t;
    }
#else // c++98 only
#   include <boost/shared_ptr.hpp>
#   include <boost/make_shared.hpp>
#   include <boost/chrono.hpp>
    namespace toml
    {
        using boost::shared_ptr;
        using boost::make_shared;
        using boost::dynamic_pointer_cast;
        using boost::chrono;
    }
#ifdef TOML_HAVE_STDINT_H
#   include <stdint.h>
    namespace toml
    {
        using ::int_least64_t;
    }
#else
#   include <boost/cstdint.hpp>
    namespace toml
    {
        using boost::int_least64_t;
    }
#endif

#endif // c++11
#endif /* TOML_CXX_CONFIG */