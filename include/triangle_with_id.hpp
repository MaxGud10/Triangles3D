#pragma once

#include <cstddef>

#include "intersection.hpp" 
#include "point.hpp"

namespace triangle
{

template <typename PointTy = double>
struct ShapeWithId
{
    Shape<PointTy> shape;
    size_t id = 0;

    Point<PointTy> min_bound{}; 
    Point<PointTy> max_bound{};

    PointTy min_x() const { return min_bound.x; }
    PointTy min_y() const { return min_bound.y; }
    PointTy min_z() const { return min_bound.z; }

    PointTy max_x() const { return max_bound.x; }
    PointTy max_y() const { return max_bound.y; }
    PointTy max_z() const { return max_bound.z; }
};

} // namespace triangle
