#pragma once

#include "point.hpp"

namespace triangle 
{

template <typename PointTy = double> 
struct Vector 
{
  static constexpr PointTy NaN = std::numeric_limits<PointTy>::quiet_NaN();

  PointTy x = NaN;
  PointTy y = NaN;
  PointTy z = NaN;

  bool valid() const 
  {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
  }

  PointTy length() const { return std::sqrt(x * x + y * y + z * z); }

  void print() const 
  {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }

  void normalize() 
  {
    PointTy len = length();
    if (!double_cmp(len, PointTy{0})) 
    {
      x = x / len;
      y = y / len;
      z = z / len;
    }
  }

  Vector<PointTy> operator+(const Vector<PointTy> &other) const 
  {
    return {x + other.x, y + other.y, z + other.z};
  }

  Vector<PointTy> operator-(const Vector<PointTy> &other) const 
  {
    return {x - other.x, y - other.y, z - other.z};
  }

  Vector<PointTy> operator/(const PointTy scalar) const 
  {
    return {x /  scalar, y / scalar, z / scalar};
  }

  Vector<PointTy> operator*(const PointTy scalar) const 
  {
    return {x * scalar, y * scalar, z * scalar};
  }

  bool operator==(const Vector&) const = default;
};

// v·w = x1x2 + y1y2 + z1z2.
template <typename PointTy = double>
PointTy dot(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

template <typename PointTy = double>
PointTy dot(const Vector<PointTy> &vector, const Point<PointTy> &point) 
{
  return vector.x * point.x + vector.y * point.y + 
         vector.z * point.z;
}

template <typename PointTy = double>
PointTy dot(const Point<PointTy> &point1, const Point<PointTy> &point2) 
{
  return point1.x * point2.x + point1.y * point2.y + 
         point1.z * point2.z;
}

// v×w = (y1z2 − z1y2, z1x2 − x1z2, x1y2 − y1x2).
template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  return { vector1.y * vector2.z - vector1.z * vector2.y,
           vector1.z * vector2.x - vector1.x * vector2.z,
           vector1.x * vector2.y - vector1.y * vector2.x
  };
}

template <typename PointTy = double>
Vector<PointTy> vector_from_point(const Point<PointTy> &point) 
{
  return { point.x, point.y, point.z };
}

template <typename PointTy = double>
Point<PointTy> operator+(const Point<PointTy> &point, const Vector<PointTy> &vector) noexcept
{
  return { point.x + vector.x, point.y + vector.y, point.z + vector.z };
}

template <typename PointTy = double>
Point<PointTy> operator-(const Point<PointTy> &point, const Vector<PointTy> &vector) noexcept
{
  return { point.x - vector.x, point.y - vector.y, point.z - vector.z };
}


} // namespace triangle