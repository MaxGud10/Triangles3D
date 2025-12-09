#pragma once

#include "triangles.hpp"

namespace triangle {

template <typename PointTy = double>
struct TriangleWithId
{
    Triangle<PointTy> tri;
    size_t id = 0;

    const Point<PointTy> &get_a() const { return tri.get_a(); }
    const Point<PointTy> &get_b() const { return tri.get_b(); }
    const Point<PointTy> &get_c() const { return tri.get_c(); }

    PointTy min_x() const { return tri.min_x(); }
    PointTy min_y() const { return tri.min_y(); }
    PointTy min_z() const { return tri.min_z(); }

    PointTy max_x() const { return tri.max_x(); }
    PointTy max_y() const { return tri.max_y(); }
    PointTy max_z() const { return tri.max_z(); }

    auto get_type() const { return tri.get_type(); }
};

} // namespace triangle
