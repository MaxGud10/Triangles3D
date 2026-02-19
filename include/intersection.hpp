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
    const Point<PointTy> p1 = l.point;
    const Point<PointTy> p2 = l.point + l.vector;

    // triangle with two coinciding vertices
    return Triangle<PointTy>(p1, p2, p2);
}

template <typename PointTy>
Shape<PointTy> make_shape_from_points(const Point<PointTy> &a,
                                      const Point<PointTy> &b,
                                      const Point<PointTy> &c)
{
    // all 3 points matched
    if (a == b && b == c)
        return a;

    // if they are collinear or have matching vertices
    if (a == b || b == c || a == c || three_points_on_one_line(a, b, c))
    {
        // we take two different points
        Point<PointTy> p = a;
        Point<PointTy> q = b;

        if (p == q)
            q = c;

        Line<PointTy> l;
        l.point  = p;
        l.vector = vector_from_point(q - p);

        return l;
    }

    // otherwise Triangle
    return Triangle<PointTy>(a, b, c);
}

// Triangle - Triangle
template <typename PointTy>
bool intersect(const Triangle<PointTy> &a, const Triangle<PointTy> &b)
{

    Triangle<PointTy> t1 = a;
    Triangle<PointTy> t2 = b;

    return intersect_triangle_with_triangle_in_3D(t1, t2);
}

// Triangle - Line
template <typename PointTy>
bool intersect(const Triangle<PointTy>& tri, const Line<PointTy>& l)
{
    Triangle<PointTy> line_tri = make_triangle_from_line(l);
    Triangle<PointTy> tcopy    = tri;

    return intersect_triangle_with_line_in_3D(tcopy, line_tri);
}

// Line - Triangle
template <typename PointTy>
bool intersect(const Line<PointTy> &l, const Triangle<PointTy> &tri)
{
    return intersect<PointTy>(tri, l);
}

// Triangle - Point
template <typename PointTy>
bool intersect(const Triangle<PointTy> &tri, const Point<PointTy> &p)
{
    Triangle<PointTy> pt_tri = make_triangle_from_point(p);
    Triangle<PointTy> tcopy  = tri;

    return intersect_triangle_with_point(tcopy, pt_tri);
}

// Point - Triangle
template <typename PointTy>
bool intersect(const Point<PointTy> &p, const Triangle<PointTy> &tri)
{
    return intersect<PointTy>(tri, p);
}

// Line - Line
template <typename PointTy>
bool intersect(const Line<PointTy> &a, const Line<PointTy> &b)
{
    Triangle<PointTy> ta = make_triangle_from_line(a);
    Triangle<PointTy> tb = make_triangle_from_line(b);

    return intersect_line_with_line(ta, tb);
}

// Line - Point
template <typename PointTy>
bool intersect(const Line<PointTy> &l, const Point<PointTy> &p)
{
    const auto cr = cross(vector_from_point(p - l.point), l.vector);

    return double_cmp(cr.x, PointTy{0}) &&
           double_cmp(cr.y, PointTy{0}) &&
           double_cmp(cr.z, PointTy{0});
}

// Point - Line
template <typename PointTy>
bool intersect(const Point<PointTy> &p, const Line<PointTy> &l)
{
    return intersect<PointTy>(l, p);
}

// Point - Point
template <typename PointTy>
bool intersect(const Point<PointTy> &a, const Point<PointTy> &b)
{
    return a == b;
}

template <typename PointTy = double>
bool check_intersection(const Shape<PointTy> &s1, const Shape<PointTy> &s2)
{
    return std::visit(
        [](auto&& a, auto&& b) -> bool { return intersect(a, b); },
        s1, s2
    );
}

} // namespace triangle