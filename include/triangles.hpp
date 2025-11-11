#pragma once

#include <iomanip>

#include "interval.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "vector.hpp"

// enum class min_x_y_z
// {
    
// };

namespace triangle 
{

template <typename PointTy = double> 
class Triangle 
{
public:
  enum TriangleType { NONE, POINT, LINE, TRIANGLE };
  size_t id = 0;

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

    else if (a == b || b == c || a == c || three_points_on_one_line(a, b, c)) 
    {
      type = LINE;
      return;
    }

    type = TRIANGLE;
  }

  PointTy coord(const Point<PointTy>& p, int axis) const 
  {
      switch (axis) 
      {
        case 0: return p.x;
        case 1: return p.y;
        case 2: return p.z;

        default: throw std::invalid_argument("Invalid axis");
      }
  }
// TODO: enum creat
  PointTy min_coord(int axis) const { return std::min({coord(a, axis), coord(b, axis), coord(c, axis)}); }
  PointTy max_coord(int axis) const { return std::max({coord(a, axis), coord(b, axis), coord(c, axis)}); }

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

  Point<PointTy> get_a() const { return a; }
  Point<PointTy> get_b() const { return b; }
  Point<PointTy> get_c() const { return c; }

  // PointTy min_x() const { return std::min(a.x, std::min(b.x, c.x)); }
  // PointTy max_x() const { return std::max(a.x, std::max(b.x, c.x)); }
  // PointTy min_y() const { return std::min(a.y, std::min(b.y, c.y)); }
  // PointTy max_y() const { return std::max(a.y, std::max(b.y, c.y)); }
  // PointTy min_z() const { return std::min(a.z, std::min(b.z, c.z)); }
  // PointTy max_z() const { return std::max(a.z, std::max(b.z, c.z)); }

  PointTy min_x() const { return min_coord(0); }
  PointTy max_x() const { return max_coord(0); }
  PointTy min_y() const { return min_coord(1); }
  PointTy max_y() const { return max_coord(1); }
  PointTy min_z() const { return min_coord(2); }
  PointTy max_z() const { return max_coord(2); }

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
    // коллинерны ли они 
    if (double_cmp( plane1.get_D(), plane2.get_D()) || 
        double_cmp(-plane1.get_D(), plane2.get_D()))
    {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }
    return false;
  }

  // расстояния вершин до плоскостей
  auto same_strict_sign = [](PointTy x, PointTy y, PointTy z) 
  {
    auto pos = [](PointTy v) { return v > 0.0 && !double_cmp(v, 0.0); };
    auto neg = [](PointTy v) { return v < 0.0 && !double_cmp(v, 0.0); };

    bool all_pos = pos(x) && pos(y) && pos(z);
    bool all_neg = neg(x) && neg(y) && neg(z);

    return all_pos || all_neg;
  };

  // если все знаковые расстояния от вершин Т2 до треугольника Т1 одного знака,
  // значит треугольники не пересекаются
  PointTy signed_dist11 = plane1.substitute(t2.get_a());
  PointTy signed_dist21 = plane1.substitute(t2.get_b());
  PointTy signed_dist31 = plane1.substitute(t2.get_c());
  if (same_strict_sign(signed_dist11, signed_dist21, signed_dist31))
    return false;

  // если все знаковые расстояния от вершин Т1 до треугольника Т2 одного знака,
  // значит треугольники не пересекаются
  PointTy signed_dist12 = plane2.substitute(t1.get_a());
  PointTy signed_dist22 = plane2.substitute(t1.get_b());
  PointTy signed_dist32 = plane2.substitute(t1.get_c());
  if (same_strict_sign(signed_dist12, signed_dist22, signed_dist32))
    return false;


  // тогда проверим на пересечение. Найдем прямую пересечения двух плоскостей
  Line<PointTy> inter_line{get_planes_intersection_vector(plane1, plane2),
                           get_planes_intersection_point (plane1, plane2)};

  // найдем интервалы пересечения треугольников с прямой пересечения плоскостей
  // Interval interval1 = get_interval_of_triangle_and_line(inter_line, t1);
  // Interval interval2 = get_interval_of_triangle_and_line(inter_line, t2);
  // TODO: [x]
  auto interval1 = get_interval_by_plane_clip(t1, plane2); // плоскость T2 режет треугольник T1
  auto interval2 = get_interval_by_plane_clip(t2, plane1); // плоскость T1 режет треугольник T2
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
  Plane<PointTy> plane(t1.get_a(), t1.get_b(), t1.get_c()); // постоили пл-ть оп t1

  // выберем какую координату исключим, чтобы рабоать в двумерных координатах
  int excluded_axis_index = 2;     

  PointTy absA = std::abs(plane.get_A()); // |A|
  PointTy absB = std::abs(plane.get_B()); // |B|
  PointTy absC = std::abs(plane.get_C()); // |C|

  // наибольшая по модулю компонента нормали указывает ось, которую выгоднее исключить
  if (absA >= absB && absA >= absC) 
    excluded_axis_index = 0; // работаем с парами (y,z)

  else if (absB >= absA && absB >= absC) 
    excluded_axis_index = 1; // работаем с парами (x,z)
   
  else 
    excluded_axis_index = 2; // // работаем с парами (x,y)


  auto project_point_to_2d = [excluded_axis_index](const Point<PointTy>& p)->std::pair<double,double>
  {
    if (excluded_axis_index == 0)  return { p.y, p.z }; // (y, z)
    if (excluded_axis_index == 1)  return { p.x, p.z }; // (x, z)
    /* excluded_axis_index == 2 */ return { p.x, p.y }; // (x, y)
  };

  // проецируем вершины обоих треугольников в 2D
  std::pair<PointTy,PointTy> a1 = project_point_to_2d(t1.get_a());
  std::pair<PointTy,PointTy> b1 = project_point_to_2d(t1.get_b());
  std::pair<PointTy,PointTy> c1 = project_point_to_2d(t1.get_c());

  std::pair<PointTy,PointTy> a2 = project_point_to_2d(t2.get_a());
  std::pair<PointTy,PointTy> b2 = project_point_to_2d(t2.get_b());
  std::pair<PointTy,PointTy> c2 = project_point_to_2d(t2.get_c());

  // ориентация площади в 2D
  auto oriented_area_2d = [](PointTy ax, PointTy ay, PointTy bx, PointTy by, PointTy cx, PointTy cy)
  {
    // >0 — слева от направленного AB; <0 — справа; =0 — коллинеарны
    return (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
  };

  // проверим лежит ли наша точка P на отрезке AB
  auto point_lies_on_segment_2d = [](PointTy ax, PointTy ay, 
                                     PointTy bx, PointTy by, 
                                     PointTy px, PointTy py)
  {
    PointTy area = (bx-ax)*(py-ay) - (by-ay)*(px-ax);
    if (!double_cmp(area, 0.0)) 
      return false; // не коллинеарны 

    PointTy minx = std::min(ax,bx), maxx = std::max(ax,bx);
    PointTy miny = std::min(ay,by), maxy = std::max(ay,by);

    return (px >= minx && px <= maxx && py >= miny && py <= maxy);
  };

  // перечение двух торезков AB и CD в 2D
  auto segments_intersect_2d = [&](PointTy ax, PointTy ay,
                                   PointTy bx, PointTy by,
                                   PointTy cx, PointTy cy,
                                   PointTy dx, PointTy dy)
  {
    PointTy o1 = oriented_area_2d(ax,ay,bx,by,cx,cy);
    PointTy o2 = oriented_area_2d(ax,ay,bx,by,dx,dy);
    PointTy o3 = oriented_area_2d(cx,cy,dx,dy,ax,ay);
    PointTy o4 = oriented_area_2d(cx,cy,dx,dy,bx,by);

    bool sep1 = (o1 > 0.0 && o2 < 0.0) || (o1 < 0.0 && o2 > 0.0);
    bool sep2 = (o3 > 0.0 && o4 < 0.0) || (o3 < 0.0 && o4 > 0.0);

    if (sep1 && sep2) return true; // отрезки пересекаются в общем положении

    // частные случаи: касание/совпадение — через проверку принадлежности концов отрезкам.
    if (point_lies_on_segment_2d(ax,ay,bx,by,cx,cy)) return true;
    if (point_lies_on_segment_2d(ax,ay,bx,by,dx,dy)) return true;
    if (point_lies_on_segment_2d(cx,cy,dx,dy,ax,ay)) return true;
    if (point_lies_on_segment_2d(cx,cy,dx,dy,bx,by)) return true;

    return false;
  };

  // проверия, что точка P=(px,py) внутри/на границе треугольника ABC в 2D
  // то есть если все ориентированные площади относительно ребер ABC имеют один и тот же знак или 0
  auto point_inside_triangle_2d = [&](PointTy px, PointTy py,
                                      const std::pair<PointTy, PointTy>& A,
                                      const std::pair<PointTy, PointTy>& B,
                                      const std::pair<PointTy, PointTy>& C)
  {
    PointTy o1 = oriented_area_2d(A.first,A.second,B.first,B.second,px,py);
    PointTy o2 = oriented_area_2d(B.first,B.second,C.first,C.second,px,py);
    PointTy o3 = oriented_area_2d(C.first,C.second,A.first,A.second,px,py);

    bool pos1 = o1 > 0.0 && !double_cmp(o1,0.0);
    bool pos2 = o2 > 0.0 && !double_cmp(o2,0.0);
    bool pos3 = o3 > 0.0 && !double_cmp(o3,0.0);
    bool neg1 = o1 < 0.0 && !double_cmp(o1,0.0);
    bool neg2 = o2 < 0.0 && !double_cmp(o2,0.0);
    bool neg3 = o3 < 0.0 && !double_cmp(o3,0.0);

    bool has_pos = pos1 || pos2 || pos3;
    bool has_neg = neg1 || neg2 || neg3;

    // если есть строго отрицательные и строго положительные => точка вне
    return !(has_pos && has_neg);
  };

  // проверка пересечения ребер 
  if (segments_intersect_2d(a1.first,a1.second, b1.first,b1.second, a2.first,a2.second, b2.first,b2.second)) return true;
  if (segments_intersect_2d(a1.first,a1.second, b1.first,b1.second, b2.first,b2.second, c2.first,c2.second)) return true;
  if (segments_intersect_2d(a1.first,a1.second, b1.first,b1.second, c2.first,c2.second, a2.first,a2.second)) return true;

  if (segments_intersect_2d(b1.first,b1.second, c1.first,c1.second, a2.first,a2.second, b2.first,b2.second)) return true;
  if (segments_intersect_2d(b1.first,b1.second, c1.first,c1.second, b2.first,b2.second, c2.first,c2.second)) return true;
  if (segments_intersect_2d(b1.first,b1.second, c1.first,c1.second, c2.first,c2.second, a2.first,a2.second)) return true;

  if (segments_intersect_2d(c1.first,c1.second, a1.first,a1.second, a2.first,a2.second, b2.first,b2.second)) return true;
  if (segments_intersect_2d(c1.first,c1.second, a1.first,a1.second, b2.first,b2.second, c2.first,c2.second)) return true;
  if (segments_intersect_2d(c1.first,c1.second, a1.first,a1.second, c2.first,c2.second, a2.first,a2.second)) return true;

  // проверим вложенность, то есть вершина одного треуг. внутри или на границе другого
  if (point_inside_triangle_2d(a1.first,a1.second, a2,b2,c2)) return true;
  if (point_inside_triangle_2d(a2.first,a2.second, a1,b1,c1)) return true;

  return false; // нет пересения 
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
    // берем две разные точки вырожденного треугольника t2 как концы сегмента
    Point<PointTy> p = t2.get_a();
    Point<PointTy> q = t2.get_b();

    // если первые две совпали
    if (p == q) 
      q = t2.get_c(); 

    return intersect_triangle_with_segment_in_2D(t1, line, p, q);
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
      if (check_two_segments_intersection(min1.x, max1.x, min2.x, max2.x) ||
          check_two_segments_intersection(min1.y, max1.y, min2.y, max2.y) ||
          check_two_segments_intersection(min1.z, max1.z, min2.z, max2.z)) 
      {
        return true;
      }
    }

    return false;
  }

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}


template <typename PointTy = double>
bool check_if_inter_point_belongs_sides(const Point<PointTy> &a1, const Point<PointTy> &b1,
                                        const Point<PointTy> &a2, const Point<PointTy> &b2,
                                        const Point<PointTy> &point) 
{
  if (!point.valid())
    return false;

  auto [min1, max1] = get_segment_space(a1, b1);
  auto [min2, max2] = get_segment_space(a2, b2);

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}

template <typename PointTy = double>
bool check_if_inter_point_belongs_space(const Point<PointTy> &min1, const Point<PointTy> &max1, const Point<PointTy> &min2, const Point<PointTy> &max2, const Point<PointTy> &point) 
{
  auto is_within_bounds = [](const Point<PointTy> &min,
                             const Point<PointTy> &max,
                             const Point<PointTy> &pt) 
  {
    return (pt.x >= min.x && pt.x <= max.x ||
            (double_cmp(pt.x, min.x)   &&
             double_cmp(pt.x, max.x))) &&

           (pt.y >= min.y && pt.y <= max.y ||
            (double_cmp(pt.y, min.y) &&
             double_cmp(pt.y, max.y))) &&

           (pt.z >= min.z && pt.z <= max.z ||
            (double_cmp(pt.z, min.z) &&
             double_cmp(pt.z, max.z)));
  };

  return is_within_bounds(min1, max1, point) &&
         is_within_bounds(min2, max2, point);
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
  PointTy min_x = std::min(t.get_a().x, std::min(t.get_b().x, t.get_c().x));
  PointTy max_x = std::max(t.get_a().x, std::max(t.get_b().x, t.get_c().x));
  PointTy min_y = std::min(t.get_a().y, std::min(t.get_b().y, t.get_c().y));
  PointTy max_y = std::max(t.get_a().y, std::max(t.get_b().y, t.get_c().y));
  PointTy min_z = std::min(t.get_a().z, std::min(t.get_b().z, t.get_c().z));
  PointTy max_z = std::max(t.get_a().z, std::max(t.get_b().z, t.get_c().z));

  Point<PointTy> min_vector{min_x, min_y, min_z};
  Point<PointTy> max_vector{max_x, max_y, max_z};

  return std::make_pair(min_vector, max_vector);
}

template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_segment_space(const Point<PointTy> a, const Point<PointTy> b) 
{
  PointTy min_x = std::min(a.x, b.x);
  PointTy max_x = std::max(a.x, b.x);
  PointTy min_y = std::min(a.y, b.y);
  PointTy max_y = std::max(a.y, b.y);
  PointTy min_z = std::min(a.z, b.z);
  PointTy max_z = std::max(a.z, b.z);

  Point<PointTy> min_vector{min_x, min_y, min_z};
  Point<PointTy> max_vector{max_x, max_y, max_z};

  return std::make_pair(min_vector, max_vector);
}

} // namespace triangle