#pragma once

#include "config.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace triangle
{

template <typename PointTy> class Triangle;

template <typename PointTy = double>
struct Line
{
  Vector<PointTy> vector; // guiding vector
  Point <PointTy> point;  // reference point P0 on the line

  bool valid() const { return vector.valid() && point.valid(); }

  void print() const
  {
    std::cout << "x = " << vector.x << "t + " << point.x << std::endl;
    std::cout << "y = " << vector.y << "t + " << point.y << std::endl;
    std::cout << "z = " << vector.z << "t + " << point.z << std::endl;
  }

  // does the point lie on a straight line
  bool is_point_on_line(const Point<PointTy> &point) const
  {
      Vector<PointTy> v  = vector_from_point(point - this->point);
      Vector<PointTy> cr = cross(v, vector);

      return double_cmp(cr.x, PointTy{0}) &&
             double_cmp(cr.y, PointTy{0}) &&
             double_cmp(cr.z, PointTy{0});
  }

  // comparing two straight lines
  bool equal(const Line<PointTy> &other) const
  {
    Vector<PointTy> cross_res = cross(vector_from_point(this->point - other.point), other.vector);

    if (double_cmp(cross_res.x, PointTy{0}) && double_cmp(cross_res.y, PointTy{0}) &&
        double_cmp(cross_res.z, PointTy{0}))
    {
      return true;
    }

    return false;
  }

  // inter_line = P0 + s*d0; line = P1 + t*d1 [in 3D]
  Point<PointTy> intersect_line_with_line(const Line<PointTy> &other) const
  {
    Point<PointTy> res{NAN, NAN, NAN};

    const Vector<PointTy> &u  = this->vector;
    const Vector<PointTy> &v  = other.vector;
          Vector<PointTy>  w0 = vector_from_point(this->point - other.point);

    PointTy A = dot(u, u);
    PointTy B = dot(u, v);
    PointTy C = dot(v, v);
    PointTy D = dot(u, w0);
    PointTy E = dot(v, w0);

    PointTy denom = A * C - B * B;
    if (double_cmp(denom, PointTy{0})) // lines are parallel
      return res;

    // non-parallel lines
    PointTy s = (B * E - C * D) / denom;
    PointTy t = (A * E - B * D) / denom;

    // points of closest approach on each straight line
    Point<PointTy> p1 = this->point + u * s;

    Point<PointTy> p2 = other.point + v * t;

    // the distance between the nearest points on straight lines
    Vector<PointTy> diff  = vector_from_point(p1 - p2);
          PointTy   dist2 = dot(diff, diff);

    // if the lines do not lie in the same plane (they are crossed) - dist2 > 0
    if (!double_cmp(dist2, PointTy{0}))
      return res;

    // lines in the same plane -> return the intersection point on the second line
    return p2;
  }

};

// we check that the 3 points lie on the same line
template <typename PointTy = double>
bool three_points_on_one_line(const Point<PointTy> &a, const Point<PointTy> &b, const Point<PointTy> &c)
{
    Vector<PointTy> AB = vector_from_point(b - a);
    Vector<PointTy> AC = vector_from_point(c - a);
    Vector<PointTy> cr = cross(AB, AC);

    return double_cmp(cr.x, PointTy{0}) && double_cmp(cr.y, PointTy{0}) && double_cmp(cr.z, PointTy{0});
}

// find the line itself (point and direction vector) from a triangle that has degenerated into a line
template <typename PointTy = double>
Line<PointTy> get_line_from_triangle(const Triangle<PointTy> &t)
{
    const auto& A = t.get_a();
    const auto& B = t.get_b();
    const auto& C = t.get_c();

    // if all three match => no line
    if (A == B && B == C)
        return {};

    Point<PointTy> p = A;
    Point<PointTy> q = B;

    if (p == q)
      q = C;

    Line<PointTy> line{};
    line.point  = p;
    line.vector = vector_from_point(q - p);

    return line;
}

// does the point lie on a straight line
template <typename PointTy = double>
bool intersect_line_with_point(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2)
{
  Line <PointTy> line  = get_line_from_triangle(t1);
  Point<PointTy> point = t2.get_a();

  Vector<PointTy> cross_res = cross(vector_from_point(point - line.point), line.vector);
  if (double_cmp(cross_res.x, PointTy{0}) && double_cmp(cross_res.y, PointTy{0}) &&
      double_cmp(cross_res.z, PointTy{0}))
  {
    return true;
  }

  return false;
}

} // namespace triangle