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
  Vector<PointTy> vector; // направляющий вектор
  Point <PointTy> point; // опорная точка P0 на линии

  bool valid() const { return vector.valid() && point.valid(); }

  void print() const 
  {
    std::cout << "x = " << vector.x << "t + " << point.x << std::endl;
    std::cout << "y = " << vector.y << "t + " << point.y << std::endl;
    std::cout << "z = " << vector.z << "t + " << point.z << std::endl;
  }
};

// проверяем, что 3 точки лежит на одной прямой
template <typename PointTy = double>
bool three_points_on_one_line(const Point<PointTy> &a, const Point<PointTy> &b, const Point<PointTy> &c)
{
    Vector<PointTy> AB = vector_from_point(b - a);
    Vector<PointTy> AC = vector_from_point(c - a);
    Vector<PointTy> cr = cross(AB, AC);

    return double_cmp(cr.x, PointTy{0}) && double_cmp(cr.y, PointTy{0}) && double_cmp(cr.z, PointTy{0});
}

// лежит ли точка на прямой
template <typename PointTy = double>
bool is_point_on_line(const Point<PointTy> &point, const Line<PointTy> &line)
{
    Vector<PointTy> v  = vector_from_point(point - line.point);
    Vector<PointTy> cr = cross(v, line.vector);

    return double_cmp(cr.x, PointTy{0}) &&
           double_cmp(cr.y, PointTy{0}) &&
           double_cmp(cr.z, PointTy{0});
}


// сравниваем две прямые 
template <typename PointTy = double>
bool equal(const Line<PointTy> &line1, const Line<PointTy> &line2) 
{
  Vector<PointTy> cross_res = cross(vector_from_point(line1.point - line2.point), line2.vector);

  if (double_cmp(cross_res.x, PointTy{0}) && double_cmp(cross_res.y, PointTy{0}) &&
      double_cmp(cross_res.z, PointTy{0}))
  {
    return true;
  }

  return false;
}


// inter_line = P0 + s*d0; line = P1 + t*d1 [in 3D]
template <typename PointTy = double>
Point<PointTy> intersect_line_with_line(const Line<PointTy> &line1, const Line<PointTy> &line2) 
{
  Point<PointTy> point{NAN, NAN, NAN};

  const Vector<PointTy>& u  = line1.vector;
  const Vector<PointTy>& v  = line2.vector;
        Vector<PointTy>  w0 = vector_from_point(line1.point - line2.point);

  PointTy A = dot(u, u);
  PointTy B = dot(u, v);
  PointTy C = dot(v, v);
  PointTy D = dot(u, w0);
  PointTy E = dot(v, w0);

  PointTy denom = A * C - B * B;
  if (double_cmp(denom, PointTy{0})) // прямые параллельны
    return point;

  // непараллельные прямые
  PointTy s = (B * E - C * D) / denom;
  PointTy t = (A * E - B * D) / denom; 

  // точки ближайшего подхода на каждой прямой
  Point<PointTy> p1 = line1.point + u * s;

  Point<PointTy> p2 = line2.point + v * t;

  // расстояние между ближайшими точками на прямых
  Vector<PointTy> diff  = vector_from_point(p1 - p2);
         PointTy  dist2 = dot(diff, diff);

  // если прямые не лежат в одной плоскости (скрещиваются) — dist2 > 0
  if (!double_cmp(dist2, PointTy{0}))
    return point;

  // прямые в одной плоскости —> возвращаем точку пересечения на второй прямой
  return p2; 
}

// находим саму линию (точку и напрявляющий вектор) из треугольника который выродился в линию
template <typename PointTy = double>
Line<PointTy> get_line_from_triangle(const Triangle<PointTy> &t)
{
    const auto& A = t.get_a();
    const auto& B = t.get_b();
    const auto& C = t.get_c();

    // if все три совпали => линии нет
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

// лежит ли точка на прямой
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