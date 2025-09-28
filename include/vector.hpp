#pragma once

#include "point.hpp"

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

    x = point.get_x();
    y = point.get_y();
    z = point.get_z();
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

  void normalize_line() 
  {
    PointTy length = length();
    x = x / length;
    y = y / length;
    z = z / length;
  }

  Vector<PointTy> operator+(const Vector<PointTy> &other) const 
  {
    Vector<PointTy> add(x + other.x, y + other.y, z + other.z);
    return add;
  }

  Vector<PointTy> operator-(const Vector<PointTy> &other) const 
  {
    Vector<PointTy> sub(x - other.x, y - other.y, z - other.z);
    return sub;
  }

  Vector<PointTy> operator*(const PointTy scalar) const 
  {
    Vector<PointTy> scalar_mul(x * scalar, y * scalar, z * scalar);
    return scalar_mul;
  }

  void operator=(const Vector<PointTy> &other) const 
  {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  void operator=(const Point<PointTy> &other) const 
  {
    x = other.x;
    y = other.y;
    z = other.z;
  }
};

template <typename PointTy = double>
bool equal(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  if (vector1.x == vector2.x && vector1.y == vector2.y &&
      vector1.z == vector2.z)
  {
    return true;
  }

  return false;
}

template <typename PointTy = double>
PointTy dot(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

template <typename PointTy = double>
PointTy dot(const Vector<PointTy> &vector, const Point<PointTy> &point) 
{
  return vector.x * point.get_x() + vector.y * point.get_y() +
         vector.z * point.get_z();
}

template <typename PointTy = double>
PointTy dot(const Point<PointTy> &point, const Vector<PointTy> &vector) 
{
  return vector.x * point.get_x() + vector.y * point.get_y() +
         vector.z * point.get_z();
}

template <typename PointTy = double>
PointTy dot(const Point<PointTy> &point1, const Point<PointTy> &point2) 
{
  return point1.get_x() * point2.get_x() + point1.get_y() * point2.get_y() +
         point1.get_z() * point2.get_z();
}

template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
  Vector<PointTy> vector(vector1.y * vector2.z - vector1.z * vector2.y,
                         vector1.z * vector2.x - vector1.x * vector2.z,
                         vector1.x * vector2.y - vector1.y * vector2.x);
                         
  return vector;
}

template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector, const Point<PointTy> &point) 
{
  Vector<PointTy> vec(vector.y * point.get_z() - vector.z * point.get_y(),
                      vector.z * point.get_x() - vector.x * point.get_z(),
                      vector.x * point.get_y() - vector.y * point.get_x());

  return vec;
}

template <typename PointTy = double>
Vector<PointTy> vector_from_point(const Point<PointTy> &point) 
{
  Vector<PointTy> vector{point.get_x(), point.get_y(), point.get_z()};

  return vector;
}