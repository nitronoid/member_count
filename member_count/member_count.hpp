#ifndef MEMBER_COUNT_HPP
#define MEMBER_COUNT_HPP
#pragma once

#include <cstdint>
#include <utility>

static_assert(__cplusplus > 201103L, "Requires at least C++14");

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

// Base case to be instantiated if the specialisation fails
template <typename, typename T, typename... Ts>
static constexpr bool is_list_initializable_impl = false;

// C++14 has no std::void_t
#if __cplusplus == 201402L
template <typename...> using void_t = void;
#else
using std::void_t;
#endif

// The compiler attempts to instantiate this first, but only succeeds if the statement is valid
template <typename T, typename... Ts>
static constexpr bool
  is_list_initializable_impl<void_t<decltype(T{{std::declval<Ts>()}...})>, T, Ts...> = true;
}  // namespace detail


// Determine whether an object of type T may be list initialised using objects of types Ts...
template <typename T, typename... Ts>
static constexpr bool is_list_initializable = detail::is_list_initializable_impl<void, T, Ts...>;

namespace detail
{
// Base declaration to be specialised, required to extract indices
template <typename T, typename>
static constexpr bool is_list_initializable_n_impl = false;

// Helper to instantiate is_list_initializable_impl with N args
template <typename T, std::size_t... I>
static constexpr bool is_list_initializable_n_impl<T, std::index_sequence<I...>> =
  is_list_initializable<T, decltype(CvtToAny{I})...>;
}  // namespace detail

// Determine whether a type T can be constructed from N arguments
template <typename T, std::size_t N>
static constexpr bool is_list_initializable_n =
  detail::is_list_initializable_n_impl<T, std::make_index_sequence<N>>;

namespace detail
{
// Base template definition to be specialised
template <typename, std::size_t, bool>
static constexpr std::size_t member_count_impl = ~std::size_t{0};

// Case hit when construction failed
template <typename T, std::size_t N>
static constexpr std::size_t member_count_impl<T, N, false> = N - 2;

// Recurse while construction succeeds
template <typename T, std::size_t N>
static constexpr std::size_t member_count_impl<T, N, true> =
  member_count_impl<T, N + 1, is_list_initializable_n<T, N>>;
}  // namespace detail

// Determine the number of members within type T
template <typename T>
static constexpr std::size_t member_count = detail::member_count_impl<T, 0, true>;

}  // namespace stdex

#include "test/member_count_tests.hpp"

#endif  // defined(__GNUC__) && !defined(__clang__)
#endif  // MEMBER_COUNT_HPP
