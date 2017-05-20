#ifndef TOML98_VALUE
#define TOML98_VALUE
#include "datetime.hpp"
#include <boost/config.hpp>
#include <boost/type_traits.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/variant.hpp>
#include <boost/cstdint.hpp>
#include <string>

namespace toml
{

struct  value;
typedef std::string                       key;
typedef bool                              Boolean;
typedef boost::int64_t                    Integer;
typedef double                            Float;
typedef std::string                       String;
typedef datetime_base<int>                Datetime;
typedef boost::container::vector<value>   Array;
typedef boost::container::map<key, value> Table;

struct value
{
    typedef boost::variant<boost::blank,
                Boolean, Integer, Float, String, Array, Table> value_type;

    value(){}
   ~value(){}
    value(const value& v) : v_(v.v_){}
    value& operator=(const value& v){this->v_ = v.v_; return *this;}

    void swap(value& v){this->v_.swap(v.v_);}

    int  which() const {return this->v_.which();}
    bool empty() const {return this->v_.empty();}
    const std::type_info& type() const {return this->v_.type();}


    value_type&       access()       {return this->v_;}
    value_type const& access() const {return this->v_;}

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    value(value&& v) : v_(std::move(v.v_)){}
    value& operator=(value&& v){this->v_ = std::move(v.v_); return *this;}

    template<typename T> value(T&& v): v_(std::forward<T>(v)){}
    template<typename T>
    value& operator=(T&& v){this->v_ = std::forward<T>(v); return *this;}
#else
    template<typename T> value(const T& v): v_(v){}
    template<typename T> value(T& v)      : v_(v){}
    template<typename T> value& operator=(const T& v){this->v_ = v; return *this;}
    template<typename T> value& operator=(T& v)      {this->v_ = v; return *this;}
#endif

  private:

    value_type v_;
};

inline void swap(value& v1, value& v2)
{
    return v1.swap(v2);
}

template<typename charT, typename traits>
inline std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const value& v)
{
    return os << v.access();
}

inline bool operator==(const value& v1, const value& v2)
{
    return (v1.access() == v2.access());
}

inline bool operator!=(const value& v1, const value& v2)
{
    return (v1.access() != v2.access());
}

inline bool operator<(const value& v1, const value& v2)
{
    return (v1.access() < v2.access());
}

inline bool operator<=(const value& v1, const value& v2)
{
    return (v1.access() <= v2.access());
}

inline bool operator>(const value& v1, const value& v2)
{
    return (v1.access() > v2.access());
}

inline bool operator>=(const value& v1, const value& v2)
{
    return (v1.access() >= v2.access());
}

} // toml
#endif// TOML98_VALUE
