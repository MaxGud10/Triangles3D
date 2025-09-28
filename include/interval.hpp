#pragma once

#include <math.h>
#include <vector>

#include "point.hpp"

template <typename PointTy> class Triangle;

template <typename PointTy = double> 
class Interval 
{
    Point<PointTy> p1;
    Point<PointTy> p2;

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
bool intersect_inervals(Interval<PointTy> &interval1, Interval<PointTy> &interval2) 
{
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

template <typename PointTy = double>
Interval<PointTy> get_valid_interval_of_triangle_and_line(const Triangle<PointTy> &triangle, const Point<PointTy> &inter_point1, const Point<PointTy> &inter_point2, const Point<PointTy> &inter_point3) 
{
    std::vector<Point<PointTy>> valid_points;

    if (is_point_in_triangle(triangle, inter_point1))
        valid_points.push_back(inter_point1);

    if (is_point_in_triangle(triangle, inter_point2))
        valid_points.push_back(inter_point2);

    if (is_point_in_triangle(triangle, inter_point3))
        valid_points.push_back(inter_point3);

    if (valid_points.size() == 2) 
    {
        return Interval<PointTy>{valid_points[0], valid_points[1]};
    } 

    else if (valid_points.size() == 3) 
    {
        if (valid_points[0] == valid_points[1])
            return Interval<PointTy>{valid_points[0], valid_points[2]};
            
        if ((valid_points[0] == valid_points[2]) ||
            (valid_points[1] == valid_points[2]))
            return Interval<PointTy>{valid_points[0], valid_points[1]};
    }

    return Interval<PointTy>{};
}