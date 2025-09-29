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

  Point<PointTy> get_a   () const { return a;    }
  Point<PointTy> get_b   () const { return b;    }
  Point<PointTy> get_c   () const { return c;    }

  TriangleType   get_type() const { return type; }
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
    return intersect_triangle_with_line_in_3D(t1, t2);
  } 
  else if (type1 == TYPE::LINE && type2 == TYPE::TRIANGLE) 
  {
    return intersect_triangle_with_line_in_3D(t2, t1);
  }

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::POINT) 
  {
    return intersect_triangle_with_point(t1, t2);
  } 
  else if (type1 == TYPE::POINT && type2 == TYPE::TRIANGLE) 
  {
    return intersect_triangle_with_point(t2, t1);
  }

  if (type1 == TYPE::LINE && type2 == TYPE::LINE) 
  {
    return intersect_line_with_line(t1, t2);
  }

  if (type1 == TYPE::LINE && type2 == TYPE::POINT) 
  {
    return intersect_line_with_point(t1, t2);
  } 
  else if (type1 == TYPE::POINT && type2 == TYPE::LINE) 
  {
    return intersect_line_with_point(t2, t1);
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

  // проверим плоскости на совпадение
  if (planes_are_parallel(plane1, plane2)) 
  {
    // коплонарны ли они
    if (( plane1.get_D() == plane2.get_D()) ||
        (-plane1.get_D() == plane2.get_D())) 
    {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }

    return false;
  }

  // если все знаковые расстояния от вершин Т2 до треугольника Т1 одного знака,
  // значит треугольники не пересекаются
  PointTy signed_dist11 = plane1.substitute(t2.get_a());
  PointTy signed_dist21 = plane1.substitute(t2.get_b());
  PointTy signed_dist31 = plane1.substitute(t2.get_c());
  if ((signed_dist11 < 0 && signed_dist21 < 0 && signed_dist31 < 0) ||
      (signed_dist11 > 0 && signed_dist21 > 0 && signed_dist31 > 0)) 
  {
    return false;
  }

  // если все знаковые расстояния от вершин Т1 до треугольника Т2 одного знака,
  // значит треугольники не пересекаются
  PointTy signed_dist12 = plane2.substitute(t1.get_a());
  PointTy signed_dist22 = plane2.substitute(t1.get_b());
  PointTy signed_dist32 = plane2.substitute(t1.get_c());
  if ((signed_dist12 < 0 && signed_dist22 < 0 && signed_dist32 < 0) ||
      (signed_dist12 > 0 && signed_dist22 > 0 && signed_dist32 > 0)) 
  {
    return false;
  }

  // тогда проверим на пересечение. Найдем прямую пересечения двух плоскостей
  Line<PointTy> inter_line{get_planes_intersection_vector(plane1, plane2),
                           get_planes_intersection_point (plane1, plane2)};

  // найдем интервалы пересечения треугольников с прямой пересечения плоскостей
  Interval interval1 = get_interval_of_triangle_and_line(inter_line, t1);
  Interval interval2 = get_interval_of_triangle_and_line(inter_line, t2);
  if (!interval1.valid() || !interval2.valid()) 
  {
    return false;
  }

  // проверим, пересекаются ли интервалы.
  return intersect_intervals(interval1, interval2);
}

template <typename PointTy = double>
Interval<PointTy> get_interval_of_triangle_and_line(const Line<PointTy> &inter_line, const Triangle<PointTy> &triangle) 
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
bool intersect_triangle_with_triangle_in_2D(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
  Line<PointTy> line1{vector_from_point(t2.get_b() - t2.get_a()), t2.get_a()};
  Line<PointTy> line2{vector_from_point(t2.get_c() - t2.get_a()), t2.get_a()};
  Line<PointTy> line3{vector_from_point(t2.get_c() - t2.get_b()), t2.get_b()};

  Point<PointTy> a = t2.get_a();
  Point<PointTy> b = t2.get_b();
  Point<PointTy> c = t2.get_c();

  if (intersect_triangle_with_segment_in_2D(t1, line1, a, b) ||
      intersect_triangle_with_segment_in_2D(t1, line2, a, c) ||
      intersect_triangle_with_segment_in_2D(t1, line3, b, c)) 
  {
    return true;
  }

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_line_in_3D(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2) 
{
  Line<PointTy> line = get_line_from_triangle(t2); // из вырожденного треугольника получаем отрезок

  Vector<PointTy> e1 = vector_from_point(t1.get_b() - t1.get_a());
  Vector<PointTy> e2 = vector_from_point(t1.get_c() - t1.get_a());

  Vector<PointTy> p  = cross(line.vector, e2);
         PointTy det = dot(p, e1);

  if (double_cmp(det, 0.0)) 
  {
    return intersect_triangle_with_segment_in_2D(t1, line, get_triangle_space(t2).first, get_triangle_space(t2).second);
  }

  Vector<PointTy> s = vector_from_point(line.point - t1.get_a());
         PointTy  u = dot(s, p) / det;
  if (u < 0.0 || u > 1.0)
    return false;

  Vector<PointTy> q = cross(s, e1);
        PointTy   v = dot(line.vector, q) / det;
  if (v < 0.0 || v > 1.0)
    return false;

  PointTy t = dot(e2, q) / det;
  if (t < 0.0 || t > 1.0)
    return false;

  if ((u + v) > 1.0)
    return false;

  return true;
}

template <typename PointTy = double>
bool intersect_triangle_with_point(const Triangle<PointTy> t1, const Triangle<PointTy> t2) 
{
  return is_point_in_triangle(t1, t2.get_a());
}

template <typename PointTy = double>
bool check_two_segments_intersection(const PointTy &min1, const PointTy &max1,
                                     const PointTy &min2, const PointTy &max2) 
{
  if (max1 < min2 || min1 > max2)
    return false;

  if (double_cmp(min1, min2) && double_cmp(min1, max1) &&
      double_cmp(min1, max2))
  {
    return false;
  }

  return true;
}


template <typename PointTy = double>
bool intersect_line_with_line(const Triangle<PointTy> t1, const Triangle<PointTy> t2) 
{
  Line<PointTy> line1 = get_line_from_triangle(t1);
  Line<PointTy> line2 = get_line_from_triangle(t2);

  Point<PointTy> point = intersect_line_with_line(line1, line2);

  auto [min1, max1] = get_triangle_space(t1);
  auto [min2, max2] = get_triangle_space(t2);

  if (!point.valid()) 
  {
    if (equal(line1, line2)) 
    {
      if (check_two_segments_intersection(min1.get_x(), max1.get_x(), min2.get_x(), max2.get_x()) ||
          check_two_segments_intersection(min1.get_y(), max1.get_y(), min2.get_y(), max2.get_y()) ||
          check_two_segments_intersection(min1.get_z(), max1.get_z(), min2.get_z(), max2.get_z())) 
      {
        return true;
      }
    }

    return false;
  }

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}


// template <typename PointTy = double>
// bool intersect_triangle_with_line_in_2D(Triangle<PointTy> &t1, Line<PointTy> &line, Point<PointTy> &t2_a, Point<PointTy> &t2_b) 
// {
//   Line<PointTy> line1{vector_from_point(t1.get_b() - t1.get_a()), t1.get_a()};
//   Line<PointTy> line2{vector_from_point(t1.get_c() - t1.get_a()), t1.get_a()};
//   Line<PointTy> line3{vector_from_point(t1.get_c() - t1.get_b()), t1.get_b()};

//   Point<PointTy> point1 = intersect_line_with_line(line, line1);
//   if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_b(), point1, t2_a, t2_b)) 
//   {
//     return true;
//   } 

//   else 
//   {
//     Point<PointTy> point2 = intersect_line_with_line(line, line2);
//     if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_c(), point2, t2_a, t2_b)) 
//       return true;

//     Point<PointTy> point3 = intersect_line_with_line(line, line3);
//     if (check_if_inter_point_belongs_side(t1.get_b(), t1.get_c(), point3, t2_a, t2_b))
//       return true;
//   }

//   return false;
// }

template <typename PointTy = double> // !
bool check_if_inter_point_belongs_sides(const Point<PointTy> &a1, const Point<PointTy> &b1, const Point<PointTy> &a2, const Point<PointTy> &b2, const Point<PointTy> &point) 
{
  if (!point.valid())
    return false;

  auto [min1, max1] = get_segment_space(a1, b1);
  auto [min2, max2] = get_segment_space(a2, b2);

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}

template <typename PointTy = double> // х
bool check_if_inter_point_belongs_space(const Point<PointTy> &min1, const Point<PointTy> &max1, const Point<PointTy> &min2, const Point<PointTy> &max2, const Point<PointTy> &point) 
{
  auto is_within_bounds = [](const Point<PointTy> &min,
                             const Point<PointTy> &max,
                             const Point<PointTy> &pt) 
  {
    return (pt.get_x() >= min.get_x() && pt.get_x() <= max.get_x()) &&
           (pt.get_y() >= min.get_y() && pt.get_y() <= max.get_y()) &&
           (pt.get_z() >= min.get_z() && pt.get_z() <= max.get_z());
  };

  return is_within_bounds(min1, max1, point) && is_within_bounds(min2, max2, point);
}


template <typename PointTy = double>
bool intersect_triangle_with_segment_in_2D(const Triangle<PointTy> &t, const Line<PointTy> &line, const Point<PointTy> &seg_min, const Point<PointTy> &seg_max) 
{
  Line<PointTy> line1{vector_from_point(t.get_b() - t.get_a()), t.get_a()};
  Point<PointTy> point1 = intersect_line_with_line(line, line1);
  if (check_if_inter_point_belongs_sides(t.get_a(), t.get_b(), seg_min, seg_max,point1)) 
  {
    return true;
  }

  Line <PointTy> line2{vector_from_point(t.get_c() - t.get_a()), t.get_a()};
  Point<PointTy> point2 = intersect_line_with_line(line, line2);
  if (check_if_inter_point_belongs_sides(t.get_a(), t.get_c(), seg_min, seg_max, point2)) 
  {
    return true;
  }

  Line<PointTy>  line3{vector_from_point(t.get_c() - t.get_b()), t.get_b()};
  Point<PointTy> point3 = intersect_line_with_line(line, line3);
  if (check_if_inter_point_belongs_sides(t.get_b(), t.get_c(), seg_min, seg_max, point3)) 
  {
    return true;
  }

  return false;
}


template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_triangle_space(const Triangle<PointTy> t) 
{
  PointTy min_x = std::min(t.get_a().get_x(), std::min(t.get_b().get_x(), t.get_c().get_x()));
  PointTy max_x = std::max(t.get_a().get_x(), std::max(t.get_b().get_x(), t.get_c().get_x()));
  PointTy min_y = std::min(t.get_a().get_y(), std::min(t.get_b().get_y(), t.get_c().get_y()));
  PointTy max_y = std::max(t.get_a().get_y(), std::max(t.get_b().get_y(), t.get_c().get_y()));
  PointTy min_z = std::min(t.get_a().get_z(), std::min(t.get_b().get_z(), t.get_c().get_z()));
  PointTy max_z = std::max(t.get_a().get_z(), std::max(t.get_b().get_z(), t.get_c().get_z()));

  Point<PointTy> min_vector{min_x, min_y, min_z};
  Point<PointTy> max_vector{max_x, max_y, max_z};

  return std::make_pair(min_vector, max_vector);
}

template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_segment_space(const Point<PointTy> a, const Point<PointTy> b) 
{
  PointTy min_x = std::min(a.get_x(), b.get_x());
  PointTy max_x = std::max(a.get_x(), b.get_x());
  PointTy min_y = std::min(a.get_y(), b.get_y());
  PointTy max_y = std::max(a.get_y(), b.get_y());
  PointTy min_z = std::min(a.get_z(), b.get_z());
  PointTy max_z = std::max(a.get_z(), b.get_z());

  Point<PointTy> min_vector{min_x, min_y, min_z};
  Point<PointTy> max_vector{max_x, max_y, max_z};

  return std::make_pair(min_vector, max_vector);
}

// template <typename PointTy = double>
// bool intersect_triangle_with_line(Triangle<PointTy> t1, Triangle<PointTy> t2) 
// {
//   Line<PointTy> line1{vector_from_point(t1.get_b() - t1.get_a()), t1.get_a()};
//   Line<PointTy> line2{vector_from_point(t1.get_c() - t1.get_a()), t1.get_a()};
//   Line<PointTy> line3{vector_from_point(t1.get_c() - t1.get_b()), t1.get_b()};

//   Line<PointTy> line {vector_from_point(t2.get_b() - t2.get_a()), t2.get_a()};
 
//   Point<PointTy> point1 = intersect_line_with_line(line, line1);
//   point1.print();


//   // if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_b(), point1, t2_a, t2_b)) 
//   // {
//   //   return true;
//   // }

//   // else 
//   // {
//   //   Point<PointTy> point2 = intersect_line_with_line(line, line2);
//   //   if (check_if_inter_point_belongs_side(t1.get_a(), t1.get_c(), point2, t2_a, t2_b)) 
//   //   {
//   //     return true;
//   //   }

//   //   Point<PointTy> point3 = intersect_line_with_line(line, line3);
//   //   if (check_if_inter_point_belongs_side(t1.get_b(), t1.get_c(), point3,
//   //   t2_a,
//   //                                         t2_b))
//   //     return true;
//   // }

//   return true;
// }