#pragma once

#include <unordered_set>
#include <vector>

#include "triangle_with_id.hpp"

namespace triag_viewer
{
int run_opengl_viewer(
    const std::vector<triangle::ShapeWithId<double>> &shapes,
    const std::unordered_set<size_t>                 &intersected_ids);
}