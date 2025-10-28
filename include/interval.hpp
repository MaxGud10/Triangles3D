#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "point.hpp"

template <typename PointTy> class Triangle;

template <typename PointTy = double> 
class Interval 
{
    Point<PointTy> p1; // первая точка интервала
    Point<PointTy> p2; // вторая точка интервала 

public:
    Interval() = default;

    Interval(Point<PointTy> p1, Point<PointTy> p2) 
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    bool valid() const { return p1.valid() && p2.valid(); }

    Point<PointTy> get_p1() const { return p1; }
    Point<PointTy> get_p2() const { return p2; }

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

    if (g_debug) {
        std::cerr << "[intersect_intervals by x]\n";
        std::cerr << "  int1: x in [" 
                << std::min(interval1.get_p1().get_x(), interval1.get_p2().get_x())
                << ", " 
                << std::max(interval1.get_p1().get_x(), interval1.get_p2().get_x()) 
                << "]\n";
        std::cerr << "  int2: x in [" 
                << std::min(interval2.get_p1().get_x(), interval2.get_p2().get_x())
                << ", " 
                << std::max(interval2.get_p1().get_x(), interval2.get_p2().get_x()) 
                << "]\n";
    }

    PointTy int1_min = std::min(interval1.get_p1().get_x(), interval1.get_p2().get_x());
    PointTy int1_max = std::max(interval1.get_p1().get_x(), interval1.get_p2().get_x());
    PointTy int2_min = std::min(interval2.get_p1().get_x(), interval2.get_p2().get_x());
    PointTy int2_max = std::max(interval2.get_p1().get_x(), interval2.get_p2().get_x());

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

// ==== NEW: надёжно строим отрезок пересечения плоскости с треугольником ====
template <typename T>
static void push_unique_point(std::vector<Point<T>>& pts, const Point<T>& p) 
{
    if (!p.valid()) 
        return;
    for (const auto& q : pts) 
    {
        if (double_cmp(p.get_x(), q.get_x()) &&
            double_cmp(p.get_y(), q.get_y()) &&
            double_cmp(p.get_z(), q.get_z())) 
            return;
    }

    pts.push_back(p);
}

template <typename T>
static std::vector<Point<T>>
clip_triangle_with_plane(const Triangle<T>& tri, const Plane<T>& pl)
{
    const Point<T> V[3] = { tri.get_a(),          tri.get_b(),          tri.get_c() };
                T  d[3] = { pl .substitute(V[0]), pl .substitute(V[1]), pl .substitute(V[2]) };
    const T eps = _epsilon;

    std::vector<Point<T>> out;
    for (int i = 0; i < 3; ++i) 
    {
        int j = (i + 1) % 3;
        const auto& P1 = V[i];
        const auto& P2 = V[j];
        T d1 = d[i], d2 = d[j];

        // точка-вершина на плоскости
        if (std::abs(d1) <= eps) push_unique_point(out, P1);
        if (std::abs(d2) <= eps) push_unique_point(out, P2);

        // разные знаки — пересечение внутри ребра
        if ((d1 > eps && d2 < -eps) || (d1 < -eps && d2 > eps)) 
        {
            T t = d1 / (d1 - d2);          // 0..1
            Point<T> I
            {
                P1.get_x() + (P2.get_x() - P1.get_x()) * t,
                P1.get_y() + (P2.get_y() - P1.get_y()) * t,
                P1.get_z() + (P2.get_z() - P1.get_z()) * t
            };
            push_unique_point(out, I);
        }
    }

    // редкие вырожденности: оставим две крайние точки
    if (out.size() > 2) 
    {
        std::sort(out.begin(), out.end(),
                  [](const auto& a, const auto& b){ return a.get_x() < b.get_x(); });

        out = { out.front(), out.back() };
    }
    return out;
}

template <typename T>
static Interval<T> interval_from_points(const std::vector<Point<T>>& pts)
{
    if (pts.empty()) return Interval<T>{};
    if (pts.size() == 1) return Interval<T>{pts[0], pts[0]};
    return Interval<T>{pts[0], pts[1]};
}

template <typename PointTy = double>
Interval<PointTy> get_valid_interval_of_triangle_and_line(
    const Triangle<PointTy> &triangle,
    const Point<PointTy> &inter_point1,
    const Point<PointTy> &inter_point2,
    const Point<PointTy> &inter_point3)
{
    std::vector<Point<PointTy>> valid_points;

    auto same_point = [](const Point<PointTy>& A, const Point<PointTy>& B) 
    {
        return double_cmp(A.get_x(), B.get_x()) &&
               double_cmp(A.get_y(), B.get_y()) &&
               double_cmp(A.get_z(), B.get_z());
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

    // Проверяем принадлежность треугольнику (границы считаются «внутри» благодаря новой is_point_in_triangle)
    if (g_debug) {
        std::cerr << "  is_point_in_triangle(AB-pt)? "
                  << (is_point_in_triangle(triangle, inter_point1) ? "YES" : "NO") << "\n";
        std::cerr << "  is_point_in_triangle(AC-pt)? "
                  << (is_point_in_triangle(triangle, inter_point2) ? "YES" : "NO") << "\n";
        std::cerr << "  is_point_in_triangle(BC-pt)? "
                  << (is_point_in_triangle(triangle, inter_point3) ? "YES" : "NO") << "\n";
    }

    if (is_point_in_triangle(triangle, inter_point1)) push_unique(inter_point1);
    if (is_point_in_triangle(triangle, inter_point2)) push_unique(inter_point2);
    if (is_point_in_triangle(triangle, inter_point3)) push_unique(inter_point3);

    if (g_debug) {
        std::cerr << "  valid_points.size() = " << valid_points.size() << "\n";
        for (size_t i = 0; i < valid_points.size(); ++i) {
            const auto& p = valid_points[i];
            std::cerr << "    vp[" << i << "] = (" << std::setprecision(12)
                      << p.get_x() << ", " << p.get_y() << ", " << p.get_z() << ")\n";
        }
    }

    // Случай 0: линия не попала в треугольник
    if (valid_points.empty()) 
    {
        return Interval<PointTy>{}; // невалидный интервал
    }

    // Случай 1: одноточечное касание (вершина/касание ребра)
    // ВАЖНО: возвращаем вырожденный интервал p–p, чтобы дальше он считался пересечением.
    if (valid_points.size() == 1) 
    {
        return Interval<PointTy>{valid_points[0], valid_points[0]};
    }

    // Случай 2: нормальное пересечение в двух точках
    if (valid_points.size() == 2) 
    {
        return Interval<PointTy>{valid_points[0], valid_points[1]};
    }

    // Случай 3: три точки — одна из них повторяет вершину.
    // Возьмём самую «дальнюю» пару (по квадрату расстояния), чтобы стабильно выбирать крайние.
    auto dist2 = [](const Point<PointTy>& A, const Point<PointTy>& B) 
    {
        const PointTy dx = A.get_x() - B.get_x();
        const PointTy dy = A.get_y() - B.get_y();
        const PointTy dz = A.get_z() - B.get_z();

        return dx*dx + dy*dy + dz*dz;
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

template <typename T>
Interval<T> get_interval_by_plane_clip(const Triangle<T>& tri, const Plane<T>& other_plane) 
{
    auto pts = clip_triangle_with_plane(tri, other_plane);
    if (g_debug) 
    {
        std::cerr << "[get_interval_by_plane_clip] points=" << pts.size() << "\n";
        for (size_t i = 0; i < pts.size(); ++i) 
        {
            std::cerr << "  p" << i << " = (" << std::setprecision(12)
                      << pts[i].get_x() << ", " << pts[i].get_y() << ", " << pts[i].get_z() << ")\n";
        }
    }
    
    return interval_from_points(pts);
}
