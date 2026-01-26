#pragma once

#include <variant>

#include "point.hpp"
#include "line.hpp"
#include "triangles.hpp"

namespace triangle
{

template <typename PointTy = double>
using Shape = std::variant<Triangle<PointTy>,
                           Line    <PointTy>,
                           Point   <PointTy>>;

template <typename PointTy>
Triangle<PointTy> make_triangle_from_point(const Point<PointTy>& p)
{
    return Triangle<PointTy>(p, p, p);
}

template <typename PointTy>
Triangle<PointTy> make_triangle_from_line(const Line<PointTy>& l)
{
    Point<PointTy> p1 = l.point;
    Point<PointTy> p2 = l.point + l.vector;

    // треугольник с двумя совпадающими вершинами
    return Triangle<PointTy>(p1, p2, p2);
}

template <typename PointTy>
Shape<PointTy> to_shape(const Triangle<PointTy>& t)
{
    using TYPE = typename Triangle<PointTy>::TriangleType;

    switch (t.get_type())
    {
        case TYPE::TRIANGLE:
            return t;
        case TYPE::LINE:
            return get_line_from_triangle(t);  // Line
        case TYPE::POINT:
            return t.get_a();                  // Point
        default: // пересечение нет
            // возвращаем просто точку (но потом check_intersection вернёт false)
            return Point<PointTy>{};
    }
}

// TRIANGLE - TRIANGLE
template <typename PointTy>
bool intersect(const Triangle<PointTy>& a, const Triangle<PointTy>& b)
{
    Triangle<PointTy> t1 = a;
    Triangle<PointTy> t2 = b;
    return intersect_triangle_with_triangle_in_3D(t1, t2);
}

// TRIANGLE — LINE
template <typename PointTy>
bool intersect(const Triangle<PointTy>& tri, const Line<PointTy>& l)
{
    Triangle<PointTy> line_tri = make_triangle_from_line(l);
    Triangle<PointTy> tcopy    = tri;
    return intersect_triangle_with_line_in_3D(tcopy, line_tri);
}

// LINE — TRIANGLE
template <typename PointTy>
bool intersect(const Line<PointTy>& l, const Triangle<PointTy>& tri)
{
    return intersect<PointTy>(tri, l);
}

// TRIANGLE — POINT
template <typename PointTy>
bool intersect(const Triangle<PointTy>& tri, const Point<PointTy>& p)
{
    Triangle<PointTy> pt_tri = make_triangle_from_point(p);
    Triangle<PointTy> tcopy  = tri;
    return intersect_triangle_with_point(tcopy, pt_tri);
}

// POINT — TRIANGLE
template <typename PointTy>
bool intersect(const Point<PointTy>& p, const Triangle<PointTy>& tri)
{
    return intersect<PointTy>(tri, p);
}

// LINE — LINE
template <typename PointTy>
bool intersect(const Line<PointTy>& a, const Line<PointTy>& b)
{
    Triangle<PointTy> ta = make_triangle_from_line(a);
    Triangle<PointTy> tb = make_triangle_from_line(b);
    return intersect_line_with_line(ta, tb);
}

// LINE — POINT
template <typename PointTy>
bool intersect(const Line<PointTy>& l, const Point<PointTy>& p)
{
    Triangle<PointTy> line_tri = make_triangle_from_line(l);
    Triangle<PointTy> pt_tri   = make_triangle_from_point(p);
    return intersect_line_with_point(line_tri, pt_tri);
}

// POINT — LINE
template <typename PointTy>
bool intersect(const Point<PointTy>& p, const Line<PointTy>& l)
{
    return intersect<PointTy>(l, p);
}

// POINT — POINT
template <typename PointTy>
bool intersect(const Point<PointTy>& a, const Point<PointTy>& b)
{
    return a == b;
}

template <typename PointTy = double>
bool check_intersection(const Triangle<PointTy>& t1,
                        const Triangle<PointTy>& t2)
{
    using TYPE = typename Triangle<PointTy>::TriangleType;

    if (t1.get_type() == TYPE::NONE || t2.get_type() == TYPE::NONE)
        return false;

    Shape<PointTy> s1 = to_shape(t1);
    Shape<PointTy> s2 = to_shape(t2);

    return std::visit(
        [](auto&& a, auto&& b) -> bool
        {
            return intersect(a, b); 
        },
        s1, s2
    );
}

} // namespace triangle