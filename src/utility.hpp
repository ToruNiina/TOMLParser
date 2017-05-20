#ifndef TOML_UTILITY
#define TOML_UTILITY
#include <boost/mpl/bool.hpp>
#include <boost/tti/has_member_function.hpp>
#include <stdexcept>

namespace toml
{

namespace detail
{

BOOST_TTI_HAS_MEMBER_FUNCTION(resize)

template<typename T>
inline void resize_impl(T& container, std::size_t N, boost::mpl::true_)
{
    container.resize(N);
    return;
}

template<typename T>
inline void resize_impl(T& container, std::size_t N, boost::mpl::false_)
{
    if(container.size() >= N) return;
    else throw std::out_of_range("not resizable type not having enough size");
}

}// detail

template<typename T>
inline void
resize(T& container, std::size_t N)
{
    return detail::resize_impl(container, N, typename boost::mpl::bool_<
        detail::has_member_function_resize<void (T::*)(std::size_t)>::value>());
}

}// toml
#endif /* TOML_UTILITY */
