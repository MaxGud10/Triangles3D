#pragma once

#include "point.hpp"

namespace triangle 
{

template <typename PointTy = double> 
class Vector 
{
public:
  PointTy x = NAN;
  PointTy y = NAN;
  PointTy z = NAN;

  Vector() = default;

  Vector(PointTy x, PointTy y, PointTy z) : x(x), y(y), z(z) {}

  Vector(const Point<PointTy> &point1, const Point<PointTy> &point2) 
  {
    Point<PointTy> point = point1 - point2;

    x = point.x;
    y = point.y;
    z = point.z;
  }

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
    if (!double_cmp(len, 0.0)) 
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


template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  Vector<PointTy> vector(vector1.y * vector2.z - vector1.z * vector2.y,
                         vector1.z * vector2.x - vector1.x * vector2.z,
                         vector1.x * vector2.y - vector1.y * vector2.x);

  return vector;
}

// v×w = (y1z2 − z1y2, z1x2 − x1z2, x1y2 − y1x2).
template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector, const Point<PointTy> &point) 
{
  Vector<PointTy> vec(vector.y * point.z - vector.z * point.y,
                      vector.z * point.x - vector.x * point.z,
                      vector.x * point.y - vector.y * point.x);

  return vec;
}

template <typename PointTy = double>
Vector<PointTy> vector_from_point(const Point<PointTy> &point) 
{
  Vector<PointTy> vector{point.x, point.y, point.z};

  return vector;
}

} // namespace triangle