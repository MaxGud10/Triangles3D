#pragma once

#include "vector.hpp"
#include "point.hpp"

template <typename PointTy = double> 
class Line 
{
public:
    Vector<PointTy> vector;
    Point<PointTy> point;

    Line() = default;
    Line(Vector<PointTy> v, Point<PointTy> p) : vector(v), point(p) {}

    void print() const 
    {
      std::cout << "x = " << vector.x << "t + " << point.get_x() << std::endl;
      std::cout << "y = " << vector.y << "t + " << point.get_y() << std::endl;
      std::cout << "z = " << vector.z << "t + " << point.get_z() << std::endl;
    }
};