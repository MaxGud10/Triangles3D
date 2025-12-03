#pragma once

#include <variant>

#include "triangles.hpp"


namespace triangle {
// -----------------------------------------------------------------------------
// Универсальный тип Shape = Point | Line | Triangle
// -----------------------------------------------------------------------------

template <typename PointTy>
using Shape = std::variant<
    Point<PointTy>,
    Line<PointTy>,
    Triangle<PointTy>
>;


// -----------------------------------------------------------------------------
// Перегрузки intersect(...) для разных комбинаций типов
// -----------------------------------------------------------------------------

// Треугольник — треугольник
template <typename PointTy>
bool intersect(const Triangle<PointTy>& a, const Triangle<PointTy>& b)
{
    // ДЕЛАЕМ КОПИИ, потому что старая функция не принимает const&
    Triangle<PointTy> t1 = a;
    Triangle<PointTy> t2 = b;
    return intersect_triangle_with_triangle_in_3D(t1, t2);
}

// Треугольник — линия
template <typename PointTy>
bool intersect(const Triangle<PointTy>& t, const Line<PointTy>& l)
{
    // адаптируем Line → Triangle как раньше
    Triangle<PointTy> fake;
    fake.set_line(l);  // ты должен сделать такую функцию!
    Triangle<PointTy> tcopy = t;
    return intersect_triangle_with_line_in_3D(tcopy, fake);
}

// Линия — треугольник
template <typename PointTy>
bool intersect(const Line<PointTy>& l, const Triangle<PointTy>& t)
{
    return intersect(t, l);
}

// Треугольник — точка
template <typename PointTy>
bool intersect(const Triangle<PointTy>& t, const Point<PointTy>& p)
{
    Triangle<PointTy> tp;
    tp.set_point(p); // тоже нужно сделать!
    Triangle<PointTy> tc = t;
    return intersect_triangle_with_point(tc, tp);
}

// Точка — треугольник
template <typename PointTy>
bool intersect(const Point<PointTy>& p, const Triangle<PointTy>& t)
{
    return intersect(t, p);
}

// Линия — линия
template <typename PointTy>
bool intersect(const Line<PointTy>& a, const Line<PointTy>& b)
{
    Triangle<PointTy> ta, tb;
    ta.set_line(a);
    tb.set_line(b);
    return intersect_line_with_line(ta, tb);
}

// Линия — точка
template <typename PointTy>
bool intersect(const Line<PointTy>& l, const Point<PointTy>& p)
{
    Triangle<PointTy> lp, ll;
    ll.set_line(l);
    lp.set_point(p);
    return intersect_line_with_point(ll, lp);
}

// Точка — линия
template <typename PointTy>
bool intersect(const Point<PointTy>& p, const Line<PointTy>& l)
{
    return intersect(l, p);
}

// Точка — точка
template <typename PointTy>
bool intersect(const Point<PointTy>& a, const Point<PointTy>& b)
{
    return a == b;
}




// -----------------------------------------------------------------------------
// Главная функция — двойной диспетч через std::visit
// -----------------------------------------------------------------------------

template <typename PointTy>
bool check_intersection(const Shape<PointTy>& s1, const Shape<PointTy>& s2)
{
    return std::visit(
        [](auto&& a, auto&& b){ return intersect(a, b); },
        s1, s2
    );
}

template <typename PointTy>
void set_point(const Point<PointTy>& p)
{
    A = p;
    B = p;
    C = p;
    type = POINT;
}

template <typename PointTy>
void set_line(const Line<PointTy>& l)
{
    A = l.point;
    B = {l.point.x + l.vector.x,
         l.point.y + l.vector.y,
         l.point.z + l.vector.z};
    C = B;
    type = LINE;
}


}