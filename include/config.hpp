#pragma once

#include <cmath>
#include <type_traits>

namespace triangle
{

template <class T>
struct numeric_traits
{
    static_assert(std::is_floating_point_v<T>, "triangle::numeric_traits<T>: T must be floating point");

    static constexpr T epsilon = static_cast<T>(1e-6);

    static bool double_cmp(T x, T y) noexcept
    {
        return std::abs(x - y) <= epsilon;
    }
};

template <class T>
bool double_cmp(T x, T y) noexcept
{
    return numeric_traits<T>::double_cmp(x, y);
}

inline constexpr double epsilon = numeric_traits<double>::epsilon;

} // namespace triangle
