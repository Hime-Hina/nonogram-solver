//
// Created by HimeHina on 2022/10/23.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_UTILITYTEMPLATES_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_UTILITYTEMPLATES_HPP_

#include <type_traits>

template <typename T, typename = void>
struct is_pair : std::false_type {};
template <typename T>
struct is_pair<
    T,
    std::void_t<typename T::first_type, typename T::second_type>
> : std::true_type {
};
template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<
    T,
    std::void_t<
        decltype(std::declval<T>().size()),
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end())
    >
> : std::true_type {
};
template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

template <typename T, typename = void>
struct is_container : std::false_type {};
template <typename T>
struct is_container<
    T,
    std::void_t<
        typename T::value_type,
        typename T::reference,
        typename T::const_reference,
        typename T::iterator,
        typename T::const_iterator,
        typename T::size_type,
        decltype(std::declval<T>().size()),
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end())
    >
> : std::true_type {
};
template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template <typename T, typename = void>
struct is_iterator : std::false_type {};
template <typename T>
struct is_iterator<
    T,
    std::void_t<
        typename std::iterator_traits<T>::iterator_category,
        typename std::iterator_traits<T>::difference_type,
        typename std::iterator_traits<T>::value_type,
        typename std::iterator_traits<T>::pointer,
        typename std::iterator_traits<T>::reference
    >
> : std::true_type {
};
template <typename T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;

#endif //NONOGRAM_INCLUDES_UTILITIES_UTILITYTEMPLATES_HPP_
