#pragma once

#include "interval.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "vector.hpp"

template <typename PointTy = double> 
class Triangle 
{
public:
  enum TriangleType { NONE, POINT, LINE, TRIANGLE };

private:
  TriangleType type = NONE;

  Point<PointTy> a, b, c;

  void validate() 
  {
    if (!a.valid() || !b.valid() || !c.valid())
      return;

    if (a == b && b == c) 
    {
      type = POINT;
      return;
    }

    else if (a == b || b == c || a == c) 
    {
      type = LINE;
      return;
    }

    type = TRIANGLE;
  }

public:
  Triangle(const PointTy &x1, const PointTy &y1, const PointTy &z1,
           const PointTy &x2, const PointTy &y2, const PointTy &z2,
           const PointTy &x3, const PointTy &y3, const PointTy &z3)
      : a(Point<PointTy>(x1, y1, z1)), b(Point<PointTy>(x2, y2, z2)),
        c(Point<PointTy>(x3, y3, z3)) 
  {
    if (a.norm() <= b.norm()) 
    {
      std::swap(a, b);
    }

    if (a.norm() <= c.norm()) 
    {
      std::swap(a, c);
    }

    if (b.norm() <= c.norm()) 
    {
      std::swap(b, c);
    }

    validate();
  }

  Triangle(const Point<PointTy> &p1, const Point<PointTy> &p2, const Point<PointTy> &p3)
         : a(p1),                    b(p2),                    c(p3) 
  {

    if (a.norm() <= b.norm()) 
    {
      std::swap(a, b);
    }

    if (a.norm() <= c.norm()) 
    {
      std::swap(a, c);
    }

    if (b.norm() <= c.norm()) 
    {
      std::swap(b, c);
    }

    validate();
  }

  void print() const 
  {
    a.print();
    b.print();
    c.print();
  }

  Point<PointTy> get_a () const { return a;    }
  Point<PointTy> get_b () const { return b;    }
  Point<PointTy> get_c () const { return c;    }

  TriangleType get_type() const { return type; }
};

template <typename PointTy = double>
bool check_intersection(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
  using TYPE = typename Triangle<PointTy>::TriangleType;

  TYPE type1 = t1.get_type();
  TYPE type2 = t2.get_type();

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::TRIANGLE) 
  {
    return intersect_triangle_with_triangle_in_3D(t1, t2);
  }

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::LINE) 
  {
    return intersect_triangle_with_line(t1, t2);
  } 

  else if (type1 == TYPE::LINE && type2 == TYPE::TRIANGLE)
  {
    return intersect_triangle_with_line(t2, t1);
  }

  if ((type1 == TYPE::TRIANGLE && type2 == TYPE::POINT) ||
      (type1 == TYPE::POINT    && type2 == TYPE::TRIANGLE)) 
  {
    // return intersectTriangleWithPoint(t1, t2);
  }

  if (type1 == TYPE::LINE && type2 == TYPE::LINE) 
  {
    // return intersectLineWithLine(t1, t2);
  }

  if ((type1 == TYPE::LINE  && type2 == TYPE::POINT) ||
      (type1 == TYPE::POINT && type2 == TYPE::LINE)) 
  {
    // return intersectLineWithPoint(t1, t2);
  }

  if (type1 == TYPE::POINT && type2 == TYPE::POINT) 
  {
    return t1.get_a() == t2.get_a();
  }

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_3D(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
  Plane<PointTy> plane1(t1.get_a(), t1.get_b(), t1.get_c());
  Plane<PointTy> plane2(t2.get_a(), t2.get_b(), t2.get_c());

  // Проверим плоскости на совпадение.
  if (planes_are_parallel(plane1, plane2)) 
  {
    if (( plane1.get_D() == plane2.get_D()) ||
        (-plane1.get_D() == plane2.get_D())) 
    {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }

    return false;
  }

  // Если все знаковые расстояния от вершин Т2 до треугольника Т1 одного знака,
  // значит треугольники не пересекаются.
  PointTy signed_dist11 = plane1.substitute(t2.get_a());
  PointTy signed_dist21 = plane1.substitute(t2.get_b());
  PointTy signed_dist31 = plane1.substitute(t2.get_c());
  if ((signed_dist11 < 0 && signed_dist21 < 0 && signed_dist31 < 0) ||
      (signed_dist11 > 0 && signed_dist21 > 0 && signed_dist31 > 0)) 
  {
    return false;
  }

  // Если все знаковые расстояния от вершин Т1 до треугольника Т2 одного знака,
  // значит треугольники не пересекаются.
  PointTy signed_dist12 = plane2.substitute(t1.get_a());
  PointTy signed_dist22 = plane2.substitute(t1.get_b());
  PointTy signed_dist32 = plane2.substitute(t1.get_c());
  if ((signed_dist12 < 0 && signed_dist22 < 0 && signed_dist32 < 0) ||
      (signed_dist12 > 0 && signed_dist22 > 0 && signed_dist32 > 0)) 
  {
    return false;
  }

  // Тогда проверим на пересечение. Найдем прямую пересечения двух плоскостей.
  Line<PointTy> inter_line{get_planes_intersection_vector(plane1, plane2),
                           get_planes_intersection_point (plane1, plane2)};

  // Найдем интервалы пересечения треугольников с прямой пересечения плоскостей.
  Interval interval1 = get_interval_of_triangle_and_line(inter_line, t1);
  Interval interval2 = get_interval_of_triangle_and_line(inter_line, t2);
  if (!interval1.valid() || !interval2.valid()) 
  {
    return false;
  }

  // Проверим, пересекаются ли интервалы.
  return intersect_inervals(interval1, interval2);
}

template <typename PointTy = double>
Interval<PointTy>
get_interval_of_triangle_and_line(const Line<PointTy> &inter_line, const Triangle<PointTy> &triangle) 
{
  Line<PointTy> line1{vector_from_point(triangle.get_b() - triangle.get_a()), triangle.get_a()};
  Line<PointTy> line2{vector_from_point(triangle.get_c() - triangle.get_a()), triangle.get_a()};
  Line<PointTy> line3{vector_from_point(triangle.get_c() - triangle.get_b()), triangle.get_b()};

  Point<PointTy> inter_point1 = intersect_line_with_line(inter_line, line1);
  Point<PointTy> inter_point2 = intersect_line_with_line(inter_line, line2);
  Point<PointTy> inter_point3 = intersect_line_with_line(inter_line, line3);

  Interval interval = get_valid_interval_of_triangle_and_line(triangle, inter_point1, inter_point2, inter_point3);

  return interval;
}

template <typename PointTy = double>
bool check_if_inter_point_belongs_triangle(const Point<PointTy> &a, const Point<PointTy> &b, const Point<PointTy> &point) 
{
  if (!point.valid())
    return false;

  PointTy min_x = std::min(a.get_x(), b.get_x());
  PointTy max_x = std::max(a.get_x(), b.get_x());
  PointTy min_y = std::min(a.get_y(), b.get_y());
  PointTy max_y = std::max(a.get_y(), b.get_y());
  PointTy min_z = std::min(a.get_z(), b.get_z());
  PointTy max_z = std::max(a.get_z(), b.get_z());

  if ((point.get_x() >= min_x && point.get_x() <= max_x) &&
      (point.get_y() >= min_y && point.get_y() <= max_y) &&
      (point.get_z() >= min_z && point.get_z() <= max_z)) 
  {
    return true;
  }

  if (point == a || point == b)
    return true;

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_2D(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
  Line<PointTy> line1{vector_from_point(t2.get_b() - t2.get_a()), t2.get_a()};
  Line<PointTy> line2{vector_from_point(t2.get_c() - t2.get_a()), t2.get_a()};
  Line<PointTy> line3{vector_from_point(t2.get_c() - t2.get_b()), t2.get_b()};

  Point<PointTy> a = t2.get_a();
  Point<PointTy> b = t2.get_b();
  Point<PointTy> c = t2.get_c();

  if (intersect_triangle_with_line_in_2D(t1, line1, a, b) ||
      intersect_triangle_with_line_in_2D(t1, line2, a, c) ||
      intersect_triangle_with_line_in_2D(t1, line3, b, c)) 
  {
    return true;
  }

  return false;
}


template <typename PointTy = double>
bool intersect_triangle_with_line_in_2D(Triangle<PointTy> &t1, Line<PointTy> &line, Point<PointTy> &t2_a, Point<PointTy> &t2_b) 
{
  Line<PointTy> line1{vector_from_point(t1.get_b() - t1.get_a()), t1.get_a()};
  Line<PointTy> line2{vector_from_point(t1.get_c() - t1.get_a()), t1.get_a()};
  Line<PointTy> line3{vector_from_point(t1.get_c() - t1.get_b()), t1.get_b()};

  Point<PointTy> point1 = intersect_line_with_line(line, line1);
  if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_b(), point1, t2_a, t2_b)) 
  {
    return true;
  } 

  else 
  {
    Point<PointTy> point2 = intersect_line_with_line(line, line2);
    if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_c(), point2, t2_a, t2_b)) 
      return true;

    Point<PointTy> point3 = intersect_line_with_line(line, line3);
    if (check_if_inter_point_belongs_side(t1.get_b(), t1.get_c(), point3, t2_a, t2_b))
      return true;
  }

  return false;
}

template <typename PointTy = double>
bool check_if_inter_point_belongs_side(const Point<PointTy> &a1, const Point<PointTy> &b1, const Point<PointTy> &point, const Point<PointTy> &a2, const Point<PointTy> &b2) 
{
  if (!point.valid())
    return false;

  PointTy min_x1 = std::min(a1.get_x(), b1.get_x());
  PointTy max_x1 = std::max(a1.get_x(), b1.get_x());
  PointTy min_y1 = std::min(a1.get_y(), b1.get_y());
  PointTy max_y1 = std::max(a1.get_y(), b1.get_y());
  PointTy min_z1 = std::min(a1.get_z(), b1.get_z());
  PointTy max_z1 = std::max(a1.get_z(), b1.get_z());

  if ((point.get_x() >= min_x1 && point.get_x() <= max_x1) &&
      (point.get_y() >= min_y1 && point.get_y() <= max_y1) &&
      (point.get_z() >= min_z1 && point.get_z() <= max_z1))
  {

    PointTy min_x2 = std::min(a2.get_x(), b2.get_x());
    PointTy max_x2 = std::max(a2.get_x(), b2.get_x());
    PointTy min_y2 = std::min(a2.get_y(), b2.get_y());
    PointTy max_y2 = std::max(a2.get_y(), b2.get_y());
    PointTy min_z2 = std::min(a2.get_z(), b2.get_z());
    PointTy max_z2 = std::max(a2.get_z(), b2.get_z());

    if ((point.get_x() >= min_x2 && point.get_x() <= max_x2) &&
        (point.get_y() >= min_y2 && point.get_y() <= max_y2) &&
        (point.get_z() >= min_z2 && point.get_z() <= max_z2)) 
    {
      return true;
    }
  }

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_line(Triangle<PointTy> t1, Triangle<PointTy> t2) 
{
  Line<PointTy> line1{vector_from_point(t1.get_b() - t1.get_a()), t1.get_a()};
  Line<PointTy> line2{vector_from_point(t1.get_c() - t1.get_a()), t1.get_a()};
  Line<PointTy> line3{vector_from_point(t1.get_c() - t1.get_b()), t1.get_b()};

  Line<PointTy> line {vector_from_point(t2.get_b() - t2.get_a()), t2.get_a()};
 
  Point<PointTy> point1 = intersect_line_with_line(line, line1);
  point1.print();

  return true;
}