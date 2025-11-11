#pragma once

#include <iostream>
#include <cmath>   

#include "config.hpp"

namespace triangle 
{

template <typename PointTy> class Triangle;
template <typename PointTy> class Vector;
template <typename PointTy> class Plane;


template <typename PointTy = double> 
class Point
{
public:
    bool valid() const 
    {
        return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
    }

    PointTy x = NAN;
    PointTy y = NAN;
    PointTy z = NAN;

    Point() = default; 

    Point(PointTy x, PointTy y, PointTy z) : x(x), y(y), z(z) {};

    void print() const 
    {
      std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }


    PointTy norm() const { return x * x + y * y + z * z; } // |p|^2
    
    bool operator==(const Point<PointTy>& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point<PointTy>& other) const
    {
        return !(*this == other);
    }

    bool almost_equal(const Point<PointTy>& other, PointTy eps = 1e-6) const
    {
        return std::fabs(x - other.x) <= eps &&
              std::fabs(y - other.y) <= eps &&
              std::fabs(z - other.z) <= eps;
    }

    Point<PointTy> operator+(const Point<PointTy> &other) const 
    {
      Point<PointTy> add(x + other.x, y + other.y, z + other.z);
      return add;
    }


    Point<PointTy> operator-(const Point<PointTy> &other) const 
    {
      Point<PointTy> sub(x - other.x, y - other.y, z - other.z);
      return sub;
    }

    void operator=(const Point<PointTy> &other) 
    {
      x = other.x;
      y = other.y;
      z = other.z;
    }
};

// ======================================================================


// проверяем, что 3 точки лежит на одной прямой
template <typename PointTy = double>
bool three_points_on_one_line(const Point<PointTy> &a, const Point<PointTy> &b, const Point<PointTy> &c)
{
    Vector<PointTy> AB{b, a}; 
    Vector<PointTy> AC{c, a};
    Vector<PointTy> cr = cross(AB, AC);

    return double_cmp(cr.x,0.0) && double_cmp(cr.y,0.0) && double_cmp(cr.z,0.0);
}

// находится ли точка внутри треугольника
template <typename PointTy>
static bool is_point_in_triangle(const Triangle<PointTy>& tri, const Point<PointTy>& point)
{
    // проверяем копланарность
    Plane<PointTy> pl(tri.get_a(), tri.get_b(), tri.get_c());
    const auto sd = pl.substitute(point);

    if (!double_cmp(sd, 0.0)) return false;

    // барицентрические координаты ё
    const Vector<PointTy> v0{tri.get_c(), tri.get_a()}; // C - A
    const Vector<PointTy> v1{tri.get_b(), tri.get_a()}; // B - A
    const Vector<PointTy> v2{point,       tri.get_a()}; // point - A

    const PointTy dot00 = dot(v0, v0);
    const PointTy dot01 = dot(v0, v1);
    const PointTy dot02 = dot(v0, v2);
    const PointTy dot11 = dot(v1, v1);
    const PointTy dot12 = dot(v1, v2);

    // для вырожденого треуголника: проверяем попадание на одно из рёбер / вершины
    const PointTy denom = dot00 * dot11 - dot01 * dot01;
    if (double_cmp(denom, 0.0)) 
    {
        // вырожденный треугольник: point лежит на каком-то ребре/вершине?
        const auto& A = tri.get_a();
        const auto& B = tri.get_b();
        const auto& C = tri.get_c();

        auto same_point = [&](const Point<PointTy>& U, const Point<PointTy>& V)
        {
            return double_cmp(U.x, V.x) &&
                   double_cmp(U.y, V.y) &&
                   double_cmp(U.z, V.z);
        };

        // лежеит ли point на отрезке [x, y]
        auto on_segment = [&](const Point<PointTy>& X, const Point<PointTy>& Y) -> bool 
        {
            const Vector<PointTy> XY{Y, X};
            const Vector<PointTy> XP{point, X};

            const auto cr = cross(XY, XP);

            // коллинеарность 
            if (!double_cmp(cr.x, 0.0) ||
                !double_cmp(cr.y, 0.0) ||
                !double_cmp(cr.z, 0.0)) 
                return false;

            // точка в пределах проекционного AABB с допуском
            const PointTy minx = std::min(X.x, Y.x) - _epsilon;
            const PointTy maxx = std::max(X.x, Y.x) + _epsilon;
            const PointTy miny = std::min(X.y, Y.y) - _epsilon;
            const PointTy maxy = std::max(X.y, Y.y) + _epsilon;
            const PointTy minz = std::min(X.z, Y.z) - _epsilon;
            const PointTy maxz = std::max(X.z, Y.z) + _epsilon;

            return (point.x >= minx && point.x <= maxx &&
                    point.y >= miny && point.y <= maxy &&
                    point.z >= minz && point.z <= maxz);
        };

        return on_segment (A,     B) || on_segment (A,     C) || on_segment (B,     C) ||
               same_point (point, A) || same_point (point, B) || same_point (point, C);
    }

    // невырожденный случай: u,v в пределах треугольника
    const PointTy inv = 1.0 / denom;
    const PointTy u   = (dot11 * dot02 - dot01 * dot12) * inv;
    const PointTy v   = (dot00 * dot12 - dot01 * dot02) * inv;

    // немного расширяем границы на eps, чтобы устойчиво считать точку на ребре «внутри»
    const PointTy eps = _epsilon * 10;
    if (u >= -eps && v >= -eps && (u + v) <= (1.0 + eps)) 
      return true;

    return (double_cmp(u, 0.0) || u > 0) &&
           (double_cmp(v, 0.0) || v > 0) &&
           (double_cmp(u + v, 1.0) || (u + v) < 1.0 + eps);
}


} // namespace triangle