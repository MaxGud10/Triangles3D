#pragma once

#include "config.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "vector.hpp"

template <typename PointTy = double> 
class Triangle 
{
public:
    enum TriangleType 
    { 
      NONE, 
      POINT, 
      LINE, 
      TRIANGLE 
    };

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
        std::swap(a, b);
      

      if (a.norm() <= c.norm()) 
        std::swap(a, c);
      

      if (b.norm() <= c.norm()) 
        std::swap(b, c);
      

      validate();
    }

    void print() const 
    {
      a.print();
      b.print();
      c.print();
    }

    Point<PointTy> get_a()    const { return a;    }
    Point<PointTy> get_b()    const { return b;    }
    Point<PointTy> get_c()    const { return c;    }

    TriangleType   get_type() const { return type; }
};

template <typename PointTy = double>
bool check_intersection(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
    using TYPE = typename Triangle<PointTy>::TriangleType;

    TYPE type1 = t1.get_type();
    TYPE type2 = t2.get_type();

    if (type1 == TYPE::TRIANGLE && type2 == TYPE::TRIANGLE) 
      return intersect_triangle_with_triangle_in_3D(t1, t2);
    
    if ((type1 == TYPE::TRIANGLE && type2 == TYPE::LINE) ||
        (type1 == TYPE::LINE     && type2 == TYPE::TRIANGLE)) 
    {
      //return intersectTriangleWithLine(t1, t2);
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
      return t1.get_a() == t2.get_a();

    return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_3D(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
    Plane<PointTy> plane1(t1.get_a(), t1.get_b(), t1.get_c()); // первая п-ть
    Plane<PointTy> plane2(t2.get_a(), t2.get_b(), t2.get_c()); // второая п-ть 

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

    // plane1.print();
    // plane1.print_normal();

    // plane2.print();
    // plane2.print_normal();

    // Vector vec = cross(plane1.get_normal(), plane2.get_normal());
    // vec.print();

    // На данном этапе плоскости треугольников либо пересекаются по линии, либо
    // совпадают. Проверим на совпадение.
    if ((plane1.get_D() == plane2.get_D()) ||
        (-plane1.get_D() == plane2.get_D())) 
    {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }

    // Тогда проверим на пересечение. Найдем прямую пересечения двух плоскостей.
    Line<PointTy> inter_line{get_planes_intersection_vector(plane1, plane2),
                             get_planes_intersection_point (plane1, plane2)};
    inter_line.print();

    // Найдем интервалы пересечения треугольников с прямой пересечения плоскостей
    // Interval interval1 = get_triangle_and_line_intersection_interval();
    // Interval interval2 = get_triangle_and_line_intersection_interval();


    return true;
}


template <typename PointTy = double>
Vector<PointTy> get_planes_intersection_vector(const Plane<PointTy> &plane1, const Plane<PointTy> &plane2) 
{
  Vector vec = cross(plane1.get_normal(), plane2.get_normal()); // напрвляющий ветора d = n1хn2z

  return vec;
}

template <typename PointTy = double>
Point<PointTy> get_planes_intersection_point(const Plane<PointTy> &plane1, const Plane<PointTy> &plane2) 
{
  PointTy x, y, z = NAN;

  PointTy determinant_x_zero = plane1.get_B() * plane2.get_C() - plane1.get_C() * plane2.get_B();
  PointTy determinant_y_zero = plane1.get_A() * plane2.get_C() - plane1.get_C() * plane2.get_A();
  PointTy determinant_z_zero = plane1.get_A() * plane2.get_B() - plane1.get_B() * plane2.get_A();

  // A1x + B1y = -D1
  // A2x + B2y = -D2
  if (!double_cmp(determinant_z_zero, 0.0)) 
  {
    x = (plane1.get_B() * plane2.get_D() - plane2.get_B() * plane1.get_D()) / determinant_z_zero;
    y = (plane2.get_A() * plane1.get_D() - plane1.get_A() * plane2.get_D()) / determinant_z_zero;
    z = 0.0;
  }

  // A1x + С1z = -D1
  // A2x + С2z = -D2
  else if (!double_cmp(determinant_y_zero, 0.0)) 
  {
    x = (plane1.get_C() * plane2.get_D() - plane2.get_C() * plane1.get_D()) / determinant_y_zero;
    z = (plane2.get_A() * plane1.get_D() - plane1.get_A() * plane2.get_D()) / determinant_y_zero;
    y = 0.0;
  }

  // B1y + С1z = -D1
  // B2y + С2z = -D2
  else if (!double_cmp(determinant_x_zero, 0.0)) 
  {
    y = (plane1.get_C() * plane2.get_D() - plane2.get_C() * plane1.get_D()) / determinant_x_zero;
    z = (plane2.get_B() * plane1.get_D() - plane1.get_B() * plane2.get_D()) / determinant_x_zero;
    x = 0.0;
  } 

  if (x == NAN || y == NAN || z == NAN) 
  {
    std::cout << "Error in determining points at direct intersection of triangles!\n";
  }

  Point<PointTy> intersectionPoint = {x, y, z};
  return         intersectionPoint;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_2D(Triangle<PointTy> &t1, Triangle<PointTy> &t2) 
{
  return true;
}

