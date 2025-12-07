#pragma once

namespace math {
    template <typename T>
    constexpr T pi();

    template <>
    constexpr float pi() { return 3.1415927f; }

    template <>
    constexpr double pi() { return 3.141592653589793; }
}