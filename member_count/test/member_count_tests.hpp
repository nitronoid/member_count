#ifndef MEMBER_COUNT_TESTS_HPP
#define MEMBER_COUNT_TESTS_HPP
#pragma once

// For standalone testing
#ifndef MEMBER_COUNT_HPP
#include "../member_count.hpp"
#endif

namespace stdex
{
namespace detail
{
namespace test
{
struct Test0
{
};
static_assert(stdex::member_count_v<Test0> == 0, "member_count unit test failed.");

struct Test1
{
    uint32_t a;
};
static_assert(stdex::member_count_v<Test1> == 1, "member_count unit test failed.");

struct Test2
{
    uint32_t a, b;
};
static_assert(stdex::member_count_v<Test2> == 2, "member_count unit test failed.");

struct Test3
{
    uint32_t a, b;
    uint64_t c;
};
static_assert(stdex::member_count_v<Test3> == 3, "member_count unit test failed.");

struct Test10
{
    uint32_t x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
};
static_assert(stdex::member_count_v<Test10> == 10, "member_count unit test failed.");

struct TestSimpleUnion
{
    union
    {
        float f;
        uint32_t i;
    };
};
static_assert(stdex::member_count_v<TestSimpleUnion> == 1, "member_count unit test failed.");

struct TestComplexUnion
{
    uint32_t x;
    union
    {
        struct
        {
            uint32_t a, b;
        };
        uint64_t c;
        double d;
    };
    uint32_t y;
};
static_assert(stdex::member_count_v<TestComplexUnion> == 3, "member_count unit test failed.");

struct Nested
{
    struct
    {
        uint32_t a, b, c;
    } inner;
};
static_assert(stdex::member_count_v<Nested> == 1, "member_count unit test failed.");

template <typename T>
struct Template
{
    T x;
    T y;
};
static_assert(stdex::member_count_v<Template<int>> == 2, "member_count unit test failed.");
}  // namespace test
}  // namespace detail
}  // namespace stdex
#endif  // MEMBER_COUNT_TESTS_HPP
