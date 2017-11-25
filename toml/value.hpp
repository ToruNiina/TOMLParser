#ifndef TOML98_VALUE_HPP
#define TOML98_VALUE_HPP
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 15
#include <boost/version.hpp>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#if BOOST_VERSION >= 105600
#include <boost/core/enable_if.hpp>
#else
#include <boost/utility/enable_if.hpp>
#endif
#include <boost/type_traits.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/swap.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <utility>
#include <string>

namespace toml
{
typedef std::string key;
struct value;

typedef bool                              boolean;
typedef boost::int64_t                    integer;
typedef double                            floating;
typedef std::string                       string;
typedef boost::gregorian::date            date;
typedef boost::posix_time::time_duration  time;
typedef std::pair<date, time>             datetime;
typedef boost::container::vector<value>   array;
typedef boost::container::map<key, value> table;

template<typename T> struct is_toml_type : boost::false_type{};
template<typename T> struct is_toml_type<T&>          : is_toml_type<T>{};
template<typename T> struct is_toml_type<T const>     : is_toml_type<T>{};
template<typename T> struct is_toml_type<T volatile>  : is_toml_type<T>{};
template<typename T> struct is_toml_type<T const&>    : is_toml_type<T>{};
template<typename T> struct is_toml_type<T volatile&> : is_toml_type<T>{};
template<> struct is_toml_type<boolean > : boost::true_type {};
template<> struct is_toml_type<integer > : boost::true_type {};
template<> struct is_toml_type<floating> : boost::true_type {};
template<> struct is_toml_type<string  > : boost::true_type {};
template<> struct is_toml_type<date    > : boost::true_type {};
template<> struct is_toml_type<time    > : boost::true_type {};
template<> struct is_toml_type<datetime> : boost::true_type {};
template<> struct is_toml_type<array   > : boost::true_type {};
template<> struct is_toml_type<table   > : boost::true_type {};

template<typename T, typename U> struct is_convertible_to : boost::false_type{};

template<typename T> struct is_convertible_to<T, boolean>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, bool>{};
template<typename T> struct is_convertible_to<T, integer>
    : boost::mpl::and_<
        boost::is_integral<typename boost::remove_cv_ref<T>::type>,
        boost::mpl::not_<
            boost::is_same<typename boost::remove_cv_ref<T>::type, bool>
        >
    >{};
template<typename T> struct is_convertible_to<T, floating>
    : boost::is_floating_point<typename boost::remove_cv_ref<T>::type>{};
template<typename T> struct is_convertible_to<T, string>
    : boost::mpl::or_<
        boost::is_same<typename boost::remove_cv_ref<T>::type, const char*>,
        boost::is_same<typename boost::remove_cv_ref<T>::type, std::string>
    >{};
template<typename T> struct is_convertible_to<T, date>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, date>{};
template<typename T> struct is_convertible_to<T, time>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, time>{};
template<typename T> struct is_convertible_to<T, datetime>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, datetime>{};
template<typename T> struct is_convertible_to<T, array>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, array>{};
template<typename T> struct is_convertible_to<T, table>
    : boost::is_same<typename boost::remove_cv_ref<T>::type, table>{};
template<typename T> struct is_convertible_to<T, value>
    : boost::mpl::or_<
        boost::is_same<typename boost::remove_cv_ref<T>::type, value>,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, boolean >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, integer >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, floating>,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, string  >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, date    >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, time    >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, datetime>,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, array   >,
        is_convertible_to<typename boost::remove_cv_ref<T>::type, table   >
    >{};

struct value
{
    typedef boost::variant<boost::blank,
        boolean, integer, floating, string, date, time, datetime, array, table
        > storage_type;

    template<typename T, typename U>
    struct if_convertible_to : boost::enable_if<is_convertible_to<T, U> >{};

    template<typename T> struct first_type_of{};
    template<typename T1, typename T2>
    struct first_type_of<std::pair<T1, T2> >{typedef T1 type;};
    template<typename T> struct second_type_of{};
    template<typename T1, typename T2>
    struct second_type_of<std::pair<T1, T2> >{typedef T2 type;};

    enum kind
    {
        empty_tag    = 0,
        boolean_tag  = 1,
        integer_tag  = 2,
        float_tag    = 3,
        string_tag   = 4,
        date_tag     = 5,
        time_tag     = 6,
        datetime_tag = 7,
        array_tag    = 8,
        table_tag    = 9
    };

    template<typename T> struct to_kind {const static kind v = undefined;};

    value(){}
    ~value(){}
    value(const value& v): storage_(v.storage_){}
    value& operator=(const value& v){storage_ = v.storage_; return *this;}

    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, boolean>::type* = 0)
        : storage_(static_cast<boolean>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, integer>::type* = 0)
        : storage_(static_cast<integer>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, floating>::type* = 0)
        : storage_(static_cast<floating>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, string>::type* = 0)
        : storage_(v)
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, date>::type* = 0)
        : storage_(static_cast<date>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, time>::type* = 0)
        : storage_(static_cast<time>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, datetime>::type* = 0)
        : storage_(static_cast<datetime>(v))
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, array>::type* = 0)
        : storage_(v)
    {}
    template<typename T>
    explicit value(const T& v, typename if_convertible_to<T, table>::type* = 0)
        : storage_(v)
    {}

    template<typename Iterator>
    value(Iterator first, Iterator last, typename if_convertible_to<
            typename boost::iterator_value<Iterator>::type, value>::type* = 0)
        : storage_(array(first, last))
    {}

    template<typename Iterator>
    value(Iterator first, Iterator last, typename boost::enable_if<
        boost::mpl::and_<
            boost::is_same<typename first_type_of<
                typename boost::iterator_value<Iterator>::type>::type, key>,
            is_convertible_to<typename second_type_of<
                typename boost::iterator_value<Iterator>::type>::type, value>
        > >::type* = 0)
        : storage_(table(first, last))
    {}

    template<typename T>
    typename boost::enable_if<is_convertible_to<T, boolean>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<boolean>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, integer>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<integer>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, floating>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<floating>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, string>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = string(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, date>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<date>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, time>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<time>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, datetime>, value>::type&
    operator=(const T& v)
    {
        this->storage_ = static_cast<datetime>(v);
        return *this;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, array>, value>::type&
    operator=(const T& v, typename if_convertible_to<T, array>::type* = 0)
    {
        this->storage_ = v;
    }
    template<typename T>
    typename boost::enable_if<is_convertible_to<T, table>, value>::type&
    operator=(const T& v, typename if_convertible_to<T, table>::type* = 0)
    {
        this->storage_ = v;
        return *this;
    }
    // XXX enable to substitute other kind of containers?

    kind which() const throw() {return static_cast<kind>(storage_.which());}
    int  index() const throw() {return storage_.which();}
    bool empty() const throw() {return storage_.empty();}

    const std::type_info& type() const {return storage_.type();}

    template<typename T>
    typename boost::enable_if<is_toml_type<T>, bool>::type
    is() const throw() {return to_kind<T>::v == this->which();}

    template<typename T>
    typename boost::enable_if<is_toml_type<T>, T>::type&
    get()       {return boost::get<T>(this->storage_);}
    template<typename T>
    typename boost::enable_if<is_toml_type<T>, T>::type const&
    get() const {return boost::get<T>(this->storage_);}

    void swap(value& rhs) {boost::swap(lhs.storage_, rhs.storage_);}

  private:

    storage_type storage_;

    friend bool operator==(const value&, const value&);
    friend bool operator> (const value&, const value&);
};

template<> struct value::to_kind<boolean >{const static kind v = boolean_tag; };
template<> struct value::to_kind<integer >{const static kind v = integer_tag; };
template<> struct value::to_kind<floating>{const static kind v = float_tag;   };
template<> struct value::to_kind<string  >{const static kind v = string_tag;  };
template<> struct value::to_kind<date    >{const static kind v = date_tag;    };
template<> struct value::to_kind<time    >{const static kind v = time_tag;    };
template<> struct value::to_kind<datetime>{const static kind v = datetime_tag;};
template<> struct value::to_kind<array   >{const static kind v = array_tag;   };
template<> struct value::to_kind<table   >{const static kind v = table_tag;   };

namespace detail
{
struct value_equality : boost::static_visitor<bool>
{
    template<typename T>
    bool operator()(const T& lhs, const T& rhs) const throw()
    {
        return lhs == rhs;
    }
};

struct value_less_than : boost::static_visitor<bool>
{
    template<typename T>
    bool operator()(const T& lhs, const T& rhs) const throw()
    {
        return lhs < rhs;
    }
};
} // detail

inline bool operator==(const value& lhs, const value& rhs)
{
    if(lhs.index() == rhs.index())
    {
        return boost::apply_visitor(detail::value_equality(),
                lhs.storage_, rhs.storage_);
    }
    else
    {
        return false;
    }
}

inline bool operator!=(const value& lhs, const value& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const value& lhs, const value& rhs)
{
    if(lhs.index() == rhs.index())
    {
        return boost::apply_visitor(detail::value_less_than(),
                lhs.storage_, rhs.storage_);
    }
    else
    {
        return lhs.index() < rhs.index();
    }
}

inline bool operator<=(const value& lhs, const value& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

inline bool operator>=(const value& lhs, const value& rhs)
{
    return !(lhs < rhs);
}

inline bool operator>(const value& lhs, const value& rhs)
{
    return !(lhs <= rhs);
}

inline void swap(value& lhs, value& rhs)
{
    return lhs.swap(rhs);
}

} // toml
#endif //TOML98_VALUE_HPP
