#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "point.hpp"

namespace triangle
{

template <typename PointTy> class Triangle;
template <typename PointTy> class Plane;

template <typename PointTy = double>
class Interval
{
public:
    Point<PointTy> p1; // первая точка интервала
    Point<PointTy> p2; // вторая точка интервала

    bool valid() const { return p1.valid() && p2.valid(); }

    void print() const
    {
        std::cout << "{";
        std::cout << "(" << p1.x << ", " << p1.y << ", " << p1.z << ")";
        std::cout << ", ";
        std::cout << "(" << p2.x << ", " << p2.y << ", " << p2.z << ")";
        std::cout << "}" << std::endl;
    }

    bool intersect_intervals(Interval<PointTy> &other) const
    {
        auto [int1_min, int1_max] = std::minmax(p1.x, p2.x);
        auto [int2_min, int2_max] = std::minmax(other.p1.x, other.p2.x);

        if (double_cmp(int1_min, int2_min) || double_cmp(int1_min, int2_max) ||
            double_cmp(int1_max, int2_min) || double_cmp(int1_max, int2_max))
        {
            return true;
        }

        if ((int1_min >= int2_min && int1_min <= int2_max) ||
            (int1_max >= int2_min && int1_max <= int2_max) ||
            (int2_min >= int1_min && int2_min <= int1_max))
        {
            return true;
        }

        return false;
    }
};

template <typename PointTy>
bool same_point(const Point<PointTy> &A, const Point<PointTy> &B)
{
    return double_cmp(A.x, B.x) &&
           double_cmp(A.y, B.y) &&
           double_cmp(A.z, B.z);
}

template <typename PointTy>
void push_unique_point(std::vector<Point<PointTy>> &points, const Point<PointTy> &p)
{
    if (!p.valid())
        return;

    for (const auto &q : points)
    {
        if (same_point<PointTy>(p, q))
            return;
    }

    points.push_back(p);
}

// intersection of a triangle with a plane
template <typename PointTy>
std::vector<Point<PointTy>> clip_triangle_with_plane(const Triangle<PointTy>& tri, const Plane<PointTy>& plane)
{
    const Point<PointTy> V[3] = { tri   .get_a(),          tri   .get_b(),          tri   .get_c() };
                PointTy  d[3] = { plane .substitute(V[0]), plane .substitute(V[1]), plane .substitute(V[2]) };
    const       PointTy  eps  = epsilon;

    std::vector<Point<PointTy>> out;
    for (int i = 0; i < 3; ++i)
    {
        int j = (i + 1) % 3;

        const auto& P1 = V[i];
        const auto& P2 = V[j];

        PointTy d1 = d[i], d2 = d[j];

        if (std::abs(d1) <= eps) push_unique_point(out, P1);
        if (std::abs(d2) <= eps) push_unique_point(out, P2);

        if ((d1 > eps && d2 < -eps) || (d1 < -eps && d2 > eps))
        {
            PointTy t = d1 / (d1 - d2); // t (0,1)
            Point<PointTy> inter = P1 + (P2 - P1) * t;

            push_unique_point(out, inter);
        }
    }

    // rare degeneracies (we will leave two extreme points)
    if (out.size() > 2)
    {
        std::sort(out.begin(), out.end(),
                  [](const auto& a, const auto& b){ return a.x < b.x; });

        out = { out.front(), out.back() };
    }

    return out;
}

// form an interval from the found intersection points with the plane
template <typename PointTy>
Interval<PointTy> interval_from_points(const std::vector<Point<PointTy>> &pts)
{
    if (pts.empty())
        return Interval<PointTy>{};

    if (pts.size() == 1)
        return Interval<PointTy>{pts[0], pts[0]};

    return Interval<PointTy>{pts[0], pts[1]};
}

template <typename PointTy>
Interval<PointTy> interval_from_three_points_farthest_pair(const std::vector<Point<PointTy>> &pts)
{
    const Point<PointTy> &a = pts[0];
    const Point<PointTy> &b = pts[1];
    const Point<PointTy> &c = pts[2];

    auto dist2 = [](const Point<PointTy> &A, const Point<PointTy> &B)
    {
        return dot(A - B, A - B); // let's take the farthest pair
    };

    PointTy d_ab = dist2(a, b);
    PointTy d_ac = dist2(a, c);
    PointTy d_bc = dist2(b, c);

    if (d_ab >= d_ac && d_ab >= d_bc)
        return Interval<PointTy>{a, b};

    else if (d_ac >= d_ab && d_ac >= d_bc)
        return Interval<PointTy>{a, c};

    else
        return Interval<PointTy>{b, c};
}


template <typename PointTy>
Interval<PointTy> interval_from_valid_points(const std::vector<Point<PointTy>> &valid_points)
{
    // case 0: the line did not fall into the triangle
    if (valid_points.empty())
        return Interval<PointTy>{}; // invalid interval

    // case 1: single-point touch
    if (valid_points.size() == 1)
        return Interval<PointTy>{valid_points[0], valid_points[0]}; // return the degenerate interval p–p so that it is considered an intersection

    // case 2: a normal intersection at two points
    if (valid_points.size() == 2)
        return Interval<PointTy>{valid_points[0], valid_points[1]};

    // case 3: three points -> one of them repeats the vertex
    return interval_from_three_points_farthest_pair<PointTy>(valid_points);
}

template <typename PointTy>
std::vector<Point<PointTy>> collect_valid_points_of_triangle_and_line(const Triangle<PointTy> &triangle,  const Point<PointTy> &p1, const Point<PointTy> &p2, const Point<PointTy> &p3)
{
    std::vector<Point<PointTy>> valid_points;

    // let's check if the point is inside the triangle
    if (is_point_in_triangle(triangle, p1)) push_unique_point(valid_points, p1);
    if (is_point_in_triangle(triangle, p2)) push_unique_point(valid_points, p2);
    if (is_point_in_triangle(triangle, p3)) push_unique_point(valid_points, p3);

    return valid_points;
}

// collect an interval of three candidate points where the line intersects the edges
template <typename PointTy = double>
Interval<PointTy> get_valid_interval_of_triangle_and_line(const Triangle<PointTy> &triangle, const Point<PointTy> &inter_point1, const Point<PointTy> &inter_point2, const Point<PointTy> &inter_point3)
{
    std::vector<Point<PointTy>> valid_points =
        collect_valid_points_of_triangle_and_line<PointTy>(triangle, inter_point1, inter_point2, inter_point3);

    return interval_from_valid_points<PointTy>(valid_points);
}

template <typename PointTy>
Interval<PointTy> get_interval_by_plane_clip(const Triangle<PointTy>& tri, const Plane<PointTy>& other_plane)
{
    auto pts = clip_triangle_with_plane(tri, other_plane);

    return interval_from_points(pts);
}

} // namespace triangle
