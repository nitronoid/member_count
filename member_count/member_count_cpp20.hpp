#ifndef MEMBER_COUNT_CPP20_HPP
#define MEMBER_COUNT_CPP20_HPP
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
}

// Use a requires expression to ensure that the type T can be created using a list containing Ts...
template <typename T, typename... Ts>
concept list_initializable = requires
{
  {T{{std::declval<Ts>()}...}};
};

namespace detail
{
// Base declaration to be specialised, required to extract indices
template <typename T, typename>
constexpr bool is_list_initializable_n_impl = false;

// Helper to instantiate is_list_initializable_impl with N args
template <typename T, std::size_t... I>
constexpr bool is_list_initializable_n_impl<T, std::index_sequence<I...>> =
  list_initializable<T, decltype(CvtToAny{I})...>;
}  // namespace detail

// Determine whether a type T can be constructed from N arguments
template <typename T, std::size_t N>
concept list_initializable_n = detail::is_list_initializable_n_impl<T, std::make_index_sequence<N>>;

namespace detail
{
// Base template definition to be specialised
template <typename, std::size_t, bool>
constexpr std::size_t member_count_impl = ~std::size_t{0};

// Case hit when construction failed
template <typename T, std::size_t N>
constexpr std::size_t member_count_impl<T, N, false> = N - 2;

// Recurse while construction succeeds
template <typename T, std::size_t N>
constexpr std::size_t member_count_impl<T, N, true> =
  member_count_impl<T, N + 1, list_initializable_n<T, N>>;
}  // namespace detail

// Determine the number of members within type T
template <typename T>
constexpr std::size_t member_count = detail::member_count_impl<T, 0, true>;

}  // namespace stdex

#include "test/member_count_tests.hpp"

#endif // defined(__GNUC__) && !defined(__clang__)
#endif // MEMBER_COUNT_CPP20_HPP
