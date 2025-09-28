#pragma once

#include "config.hpp"
#include "point.hpp"
#include "vector.hpp"

template <typename PointTy = double> 
class Line 
{
public:
  Vector<PointTy> vector;
  Point<PointTy>  point;

  Line() = default;
  Line(Vector<PointTy> v, Point<PointTy> p) : vector(v), point(p) {}

  bool valid() const { return vector.valid() && point.valid(); }

  void print() const 
  {
    std::cout << "x = " << vector.x << "t + " << point.get_x() << std::endl;
    std::cout << "y = " << vector.y << "t + " << point.get_y() << std::endl;
    std::cout << "z = " << vector.z << "t + " << point.get_z() << std::endl;
  }
};

template <typename PointTy = double>
Point<PointTy> intersect_line_with_line(const Line<PointTy> &line1, const Line<PointTy> &line2) 
{
  Point<PointTy> point{NAN, NAN, NAN};

  PointTy A = dot(line1.vector, line1.vector                           );
  PointTy B = dot(line1.vector, line2.vector                           );
  PointTy C = dot(line2.vector, line2.vector                           );
  PointTy D = dot(line1.vector, line1.point - line2.point              );
  PointTy E = dot(line2.vector, line1.point - line2.point              );
  PointTy F = dot(line1.point - line2.point,  line1.point - line2.point);

  PointTy denom = A * C - B * B;
  if (double_cmp(denom, 0.0)) // Parallel lines
    return point;

  // Nonparallel lines
  PointTy s = (B * E - C * D) / denom;
  PointTy t = (B * D - A * E) / denom;

  PointTy dist = s * (A * s + B * t + 2 * D) + t * (B * s + C * t + 2 * E) + F;
  if (!double_cmp(dist, 0.0)) // Lines are not on one plane
    return point;

  // Lines on one plane
  point = {-line2.vector.x * t + line2.point.get_x(),
           -line2.vector.y * t + line2.point.get_y(),
           -line2.vector.z * t + line2.point.get_z()};

  return point;
}