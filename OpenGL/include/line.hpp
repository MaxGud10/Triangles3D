#pragma once

#include "vector.hpp"

namespace triangle 
{

template <typename PointTy> class Triangle;

template <typename PointTy = double> 
struct Line 
{
  Vector<PointTy> vector;
  Point<PointTy>  point;

  bool valid() const 
  {
    return triangle::valid(vector) && triangle::valid(point);
  }

  void print() const 
  {
    std::cout << "x = " << vector.x << "t + " << point.x << std::endl;
    std::cout << "y = " << vector.y << "t + " << point.y << std::endl;
    std::cout << "z = " << vector.z << "t + " << point.z << std::endl;
  }

  bool operator==(const Line<PointTy> &other) const = default;
};

// inter_line = P0 + s*d0; line = P1 + t*d1
template <typename PointTy = double>
Point<PointTy> intersect_line_with_line(const Line<PointTy> &line1, const Line<PointTy> &line2) 
{
  Point<PointTy> point;

  PointTy A = dot(line1.vector, line1.vector);
  PointTy B = dot(line1.vector, line2.vector);
  PointTy C = dot(line2.vector, line2.vector);
  PointTy D = dot(line1.vector, line1.point - line2.point);
  PointTy E = dot(line2.vector, line1.point - line2.point);
  PointTy F = dot(line1.point - line2.point, line1.point - line2.point);

  PointTy denom = A * C - B * B;
  if (cmp(denom, 0.0)) // Parallel lines
    return point;

  // Nonparallel lines
  PointTy s = (B * E - C * D) / denom;
  PointTy t = (B * D - A * E) / denom;

  PointTy dist = s * (A * s + B * t + 2 * D) + t * (B * s + C * t + 2 * E) + F;
  if (!cmp(dist, 0.0)) // Lines are not on one plane
    return point;

  // Lines on one plane
  Point<PointTy> tmp_point = point_from_vector(line2.vector);
  point = {line2.point.x - t * tmp_point.x, line2.point.y - t * tmp_point.y,
           line2.point.z - t * tmp_point.z};

  return point;
}

template <typename PointTy = double>
Line<PointTy> get_line_from_triangle(const Triangle<PointTy> t) 
{
  Line<PointTy> line{};

  if (t.get_type() != Triangle<PointTy>::LINE)
    return line;

  if (equal(t.get_a(), t.get_b())) 
  {
    line.vector = t.get_c() - t.get_a();
    line.point = t.get_a();
  } 

  else if (equal(t.get_a(), t.get_c())) 
  {
    line.vector = t.get_b() - t.get_a();
    line.point = t.get_a();
  } 

  else 
  {
    line.vector = t.get_c() - t.get_a();
    line.point = t.get_a();
  }

  return line;
}

template <typename PointTy = double>
bool intersect_line_with_point(const Triangle<PointTy> t1, const Triangle<PointTy> t2) 
{
  Line<PointTy> line = get_line_from_triangle(t1);
  Point<PointTy> point = t2.get_a();

  Vector<PointTy> cross_res = cross(point - line.point, line.vector);
  if (cmp(cross_res.x, 0.0) && cmp(cross_res.y, 0.0) && cmp(cross_res.z, 0.0))
    return true;

  return false;
}
} // namespace triangle