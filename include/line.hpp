#pragma once

#include "config.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace triangle 
{

template <typename PointTy> class Triangle;

template <typename PointTy = double> 
class Line 
{
public:
  Vector<PointTy> vector; // направляющий вектор
  Point<PointTy>  point;  // опорная точка P0 на линии

  // Line() = default;
  // Line(Vector<PointTy> v, Point<PointTy> p) : vector(v), point(p) {} // L(t) = P0 + t * d, где P0 = point, d = vector, t — вещественное число

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
    Vector<PointTy> AB{b, a}; 
    Vector<PointTy> AC{c, a};
    Vector<PointTy> cr = cross(AB, AC);

    return double_cmp(cr.x,0.0) && double_cmp(cr.y,0.0) && double_cmp(cr.z,0.0);
}

// лежит ли точка на прямой
template <typename PointTy = double>
bool is_point_on_line(Point<PointTy> &point, Line<PointTy> &line) 
{
  if (double_cmp(line.vector.x * point.x + line.point.x, 0.0) &&
      double_cmp(line.vector.y * point.y + line.point.y, 0.0) &&
      double_cmp(line.vector.z * point.z + line.point.z, 0.0)) 
  {
    return true;
  }

  return false;
}


// сравниваем две прямые 
template <typename PointTy = double>
bool equal(Line<PointTy> &line1, Line<PointTy> &line2) 
{
  Vector<PointTy> cross_res = cross(vector_from_point(line1.point - line2.point), line2.vector);

  if (double_cmp(cross_res.x, 0.0) && double_cmp(cross_res.y, 0.0) &&
      double_cmp(cross_res.z, 0.0))
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
  if (double_cmp(denom, 0.0)) // прямые параллельны
    return point;

  // непараллельные прямые
  PointTy s = (B * E - C * D) / denom;
  PointTy t = (A * E - B * D) / denom; 

  // точки ближайшего подхода на каждой прямой
  Point<PointTy> p1
  {
      line1.point.x + u.x * s,
      line1.point.y + u.y * s,
      line1.point.z + u.z * s,
  };

  Point<PointTy> p2
  {
      line2.point.x + v.x * t,
      line2.point.y + v.y * t,
      line2.point.z + v.z * t,
  };

  // расстояние между ближайшими точками на прямых
  Vector<PointTy> diff  = vector_from_point(p1 - p2);
         PointTy  dist2 = dot(diff, diff);

  // если прямые не лежат в одной плоскости (скрещиваются) — dist2 > 0
  if (!double_cmp(dist2, 0.0))
    return point;

  // прямые в одной плоскости —> возвращаем точку пересечения на второй прямой
  return p2; 
}

// находим саму линию (точку и напрявляющий вектор) из треугольника который выродился в линию
template <typename PointTy = double>
Line<PointTy> get_line_from_triangle(const Triangle<PointTy> t) 
{
  Line<PointTy> line{};

  if (t.get_type() != Triangle<PointTy>::LINE)
    return line;

  if (t.get_a() == t.get_b()) 
  {
    line.vector = vector_from_point(t.get_c() - t.get_a()); // напрвл вект d = c - a, P0 = a
    line.point  = t.get_a();
  } 

  else if (t.get_a() == t.get_c()) 
  {
    line.vector = vector_from_point(t.get_b() - t.get_a()); // d = b − a, P0 = a
    line.point  = t.get_a();
  } 

  else // b == c
  {
    line.vector = vector_from_point(t.get_c() - t.get_a());
    line.point  = t.get_a();
  }

  return line;
}

// лежит ли точка на прямой
template <typename PointTy = double>
bool intersect_line_with_point(const Triangle<PointTy> t1, const Triangle<PointTy> t2) 
{
  Line <PointTy> line  = get_line_from_triangle(t1);
  Point<PointTy> point = t2.get_a();

  Vector<PointTy> cross_res = cross(vector_from_point(point - line.point), line.vector);
  if (double_cmp(cross_res.x, 0.0) && double_cmp(cross_res.y, 0.0) &&
      double_cmp(cross_res.z, 0.0)) 
  {
    return true;
  }

  return false;
}

} // namespace triangle