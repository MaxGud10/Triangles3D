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
        p1.print_no_endl();

        std::cout << ", ";
        p2.print_no_endl();

        std::cout << "}" << std::endl;
    }
};

template <typename PointTy = double>
bool intersect_intervals(Interval<PointTy> &interval1, Interval<PointTy> &interval2)
{
    auto [int1_min, int1_max] = std::minmax(interval1.p1.x, interval1.p2.x);
    auto [int2_min, int2_max] = std::minmax(interval2.p1.x, interval2.p2.x);
    
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

// ====================================================
template <typename PointTy>
void push_unique_point(std::vector<Point<PointTy>>& points, const Point<PointTy>& p)
{
    if (!p.valid())
        return;

    for (const auto& q : points)
    {
        if (double_cmp(p.x, q.x) &&
            double_cmp(p.y, q.y) &&
            double_cmp(p.z, q.z))
            return;
    }

    points.push_back(p);
}

// пересечение треугольника с плоскоcтью
template <typename PointTy>
std::vector<Point<PointTy>> clip_triangle_with_plane(const Triangle<PointTy>& tri, const Plane<PointTy>& plane)
{
    const Point<PointTy> V[3] = { tri   .get_a(),          tri   .get_b(),          tri    .get_c() };
                PointTy  d[3] = { plane .substitute(V[0]), plane .substitute(V[1]), plane .substitute(V[2]) };
    const       PointTy  eps  = epsilon;

    std::vector<Point<PointTy>> out;
    for (int i = 0; i < 3; ++i)
    {
        int j = (i + 1) % 3;

        const auto& P1 = V[i];
        const auto& P2 = V[j];

        PointTy d1 = d[i], d2 = d[j];

        // тыершины лежащие на плоскости
        if (std::abs(d1) <= eps) push_unique_point(out, P1);
        if (std::abs(d2) <= eps) push_unique_point(out, P2);

        // разные знаки —> ребро пересекает плоскость
        if ((d1 > eps && d2 < -eps) || (d1 < -eps && d2 > eps))
        {
            PointTy t = d1 / (d1 - d2); // t (0,1)
            Point<PointTy> inter = P1 + (P2 - P1) * t;

            push_unique_point(out, inter);
        }
    }

    // редкие вырожденности (оставим две крайние точки)
    if (out.size() > 2)
    {
        std::sort(out.begin(), out.end(),
                  [](const auto& a, const auto& b){ return a.x < b.x; });

        out = { out.front(), out.back() };
    }

    return out;
}

// фломируем интервал из найденных точек пересечения с плоскостью
template <typename PointTy>
Interval<PointTy> interval_from_points(const std::vector<Point<PointTy>>& pts)
{
    if (pts.empty())
        return Interval<PointTy>{};

    if (pts.size() == 1)
        return Interval<PointTy>{pts[0], pts[0]};

    return Interval<PointTy>{pts[0], pts[1]};
}

// собраем интервал из трёх кандидатных точек пересечения прямой с ребрами
template <typename PointTy = double>
Interval<PointTy> get_valid_interval_of_triangle_and_line( const Triangle<PointTy> &triangle, const Point<PointTy> &inter_point1, const Point<PointTy> &inter_point2, const Point<PointTy> &inter_point3)
{
    std::vector<Point<PointTy>> valid_points;

    auto same_point = [](const Point<PointTy>& A, const Point<PointTy>& B)
    {
        return double_cmp(A.x, B.x) &&
               double_cmp(A.y, B.y) &&
               double_cmp(A.z, B.z);
    };

    auto push_unique = [&](const Point<PointTy>& P)
    {
        if (!P.valid())
            return;

        for (const auto& Q : valid_points)
        {
            if (same_point(P, Q))
                return; // уже есть
        }

        valid_points.push_back(P);
    };

    // проверим внутри ли точка треугольник
    if (is_point_in_triangle(triangle, inter_point1)) push_unique(inter_point1);
    if (is_point_in_triangle(triangle, inter_point2)) push_unique(inter_point2);
    if (is_point_in_triangle(triangle, inter_point3)) push_unique(inter_point3);


    // case 0: линия не попала в треугольник
    if (valid_points.empty())
    {
        return Interval<PointTy>{}; // невалидный интервал
    }

    // case 1: одноточечное касание
    if (valid_points.size() == 1)
    {
        return Interval<PointTy>{valid_points[0], valid_points[0]}; // возвращаем вырожденный интервал p–p, чтобы дальше он считался пересечением
    }

    // case 2: нормальное пересечение в двух точках
    if (valid_points.size() == 2)
    {
        return Interval<PointTy>{valid_points[0], valid_points[1]};
    }

    // case 3: три точки —> одна из них повторяет вершину
    auto dist2 = [](const Point<PointTy>& A, const Point<PointTy>& B)
    {
        return dot(A - B, A - B); // возьмём самую дальнюю пару
    };

    Point<PointTy> a = valid_points[0];
    Point<PointTy> b = valid_points[1];
    Point<PointTy> c = valid_points[2];

    PointTy d_ab = dist2(a,b);
    PointTy d_ac = dist2(a,c);
    PointTy d_bc = dist2(b,c);

    if (d_ab >= d_ac && d_ab >= d_bc)
        return Interval<PointTy>{a, b};

    else if (d_ac >= d_ab && d_ac >= d_bc)
        return Interval<PointTy>{a, c};

    else
        return Interval<PointTy>{b, c};

}


template <typename PointTy>
Interval<PointTy> get_interval_by_plane_clip(const Triangle<PointTy>& tri, const Plane<PointTy>& other_plane)
{
    auto pts = clip_triangle_with_plane(tri, other_plane);

    return interval_from_points(pts);
}

} // namespace triangle
