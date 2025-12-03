#pragma once

#include <iostream>
#include <cmath>   
#include <limits>

#include "config.hpp"

namespace triangle 
{

template <typename PointTy = double> 
class Point
{
public:
    bool valid() const 
    {
        return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
    }

    static constexpr PointTy NaN = std::numeric_limits<PointTy>::quiet_NaN();

    PointTy x = NaN;
    PointTy y = NaN;
    PointTy z = NaN;

    Point() = default; 

    Point(PointTy x, PointTy y, PointTy z) : x(x), y(y), z(z) {};

    void print() const 
    {
      std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }


    PointTy norm() const { return x * x + y * y + z * z; } // |p|^2
    
    bool operator==(const Point<PointTy>& other) const = default;
    // {
    //     return x == other.x && y == other.y && z == other.z;
    // }

    // bool operator!=(const Point<PointTy>& other) const
    // {
    //     return !(*this == other);
    // }

    bool almost_equal(const Point<PointTy>& other, PointTy eps = 1e-6) const
    {
      return double_cmp(x, other.x) && double_cmp(y, other.y) &&
             double_cmp(z, other.z);
    }

    Point<PointTy> operator+(const Point<PointTy> &other) const 
    {
      Point<PointTy> add(x + other.x, y + other.y, z + other.z);
      return add;
    }


    Point<PointTy> operator-(const Point<PointTy> &other) const 
    {
      Point<PointTy> sub(x - other.x, y - other.y, z - other.z);
      return sub;
    }
};

} // namespace triangle