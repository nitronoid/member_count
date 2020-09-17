#ifndef MEMBER_COUNT_HPP
#define MEMBER_COUNT_HPP
#pragma once

#include <cstdint>
#include <utility>

#if defined(__GNUC__) && !defined(__clang__)
static_assert(__GNUC__ > 8, "member_count requires GCC version 9 or above.");
#else

namespace stdex
{
namespace detail
{
struct CvtToAny
{
    // Produce from any sequence of values
    template <typename... T> CvtToAny(T&&...) {}
    // Don't need to define this as it should never be instantiated/used
    template <typename T> constexpr operator T() noexcept;
};

// Swallow types and yield void
template <typename...> using void_t = void;

// Base case to be instantiated if the specialisation fails
template <typename, typename T, typename... Ts>
struct is_list_initializable_impl : std::false_type {};

// The compiler attempts to instantiate this first, but only succeeds if the statement is valid
template <typename T, typename... Ts>
struct is_list_initializable_impl<void_t<decltype(T{{std::declval<Ts>()}...})>, T, Ts...> : std::true_type {};
}  // namespace detail

// Determine whether an object of type T may be list initialised using objects of types Ts...
template <typename T, typename... Ts>
struct is_list_initializable : detail::is_list_initializable_impl<void, T, Ts...> {};

template <typename T, typename... Ts>
static constexpr auto is_list_initializable_v = is_list_initializable<T, Ts...>::value;

namespace detail
{
// Base declaration to be specialised, required to extract indices
template <typename T, typename> struct is_list_initializable_n_impl;

// Helper to instantiate is_list_initializable_impl with N args
template <typename T, std::size_t... I>
struct is_list_initializable_n_impl<T, std::index_sequence<I...>>
    : std::integral_constant<bool, is_list_initializable<T, decltype(CvtToAny{I})...>::value>
{};
}  // namespace detail

// Determine whether a type T can be constructed from N arguments
template <typename T, std::size_t N>
struct is_list_initializable_n
    : std::integral_constant<bool, detail::is_list_initializable_n_impl<T, std::make_index_sequence<N>>::value>
{};

template <typename T, std::size_t N>
static constexpr auto is_list_initializable_n_v = is_list_initializable_n<T, N>::value;

namespace detail
{
// Base template definition to be specialised
template <typename, std::size_t, bool> struct member_count_impl;

// Case hit when construction failed
template <typename T, std::size_t N>
struct member_count_impl<T, N, false> : std::integral_constant<std::size_t, N> {};

// Recurse while construction succeeds
template <typename T, std::size_t N>
struct member_count_impl<T, N, true>
     : std::integral_constant<std::size_t, member_count_impl<T, N + 1, is_list_initializable_n<T, N>::value>::value>
{};
}  // namespace detail

// Determine the number of members within type T
template <typename T>
struct member_count
{
    // Sub 1 as we eagerly increment, and another due to the failure occurring at N+1 arguments
    static constexpr std::size_t value = detail::member_count_impl<T, 0, true>::value - 2;
};

template <typename T>
static constexpr auto member_count_v = member_count<T>::value;

}  // namespace stdex

#include "test/member_count_tests.hpp"

#endif // defined(__GNUC__) && !defined(__clang__)
#endif // MEMBER_COUNT_HPP
