#pragma once

#include <algorithm>
#include <deque>
#include <map>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "octotree.hpp"
#include "triangle_with_id.hpp"
#include "intersection.hpp"

namespace triag_core
{
using PointTy = double;

inline std::vector<triangle::ShapeWithId<PointTy>> read_shapes(std::istream& in)
{
    std::vector<triangle::ShapeWithId<PointTy>> input;
    size_t triag_num = 0;

    if (!(in >> triag_num))
        throw std::runtime_error("Failed to read triangle count");

    input.reserve(triag_num);

    for (size_t i = 0; i < triag_num; ++i)
    {
        PointTy x1=0,y1=0,z1=0, x2=0,y2=0,z2=0, x3=0,y3=0,z3=0;

        if (!(in >> x1 >> y1 >> z1
                 >> x2 >> y2 >> z2
                 >> x3 >> y3 >> z3))
        {
            throw std::runtime_error("Input error: see README for correct usage");
        }

        triangle::Point<PointTy> a{x1,y1,z1};
        triangle::Point<PointTy> b{x2,y2,z2};
        triangle::Point<PointTy> c{x3,y3,z3};

        triangle::ShapeWithId<PointTy> rec;
        rec.shape = triangle::make_shape_from_points<PointTy>(a,b,c);

        rec.min_bound = {std::min({a.x, b.x, c.x}),
                         std::min({a.y, b.y, c.y}),
                         std::min({a.z, b.z, c.z})};

        rec.max_bound = {std::max({a.x, b.x, c.x}),
                         std::max({a.y, b.y, c.y}),
                         std::max({a.z, b.z, c.z})};

        rec.id = i;
        input.push_back(rec);
    }

    return input;
}

inline std::vector<size_t> compute_intersected_ids_sorted(
    const std::vector<triangle::ShapeWithId<PointTy>>& input)
{
    triangle::Octotree<PointTy> octotree(input);
    octotree.divide_tree();

    std::map<size_t, size_t> result;

    const auto& cells = octotree.get_cells();
    for (const auto& cell : cells)
        cell.group_intersections(result);

    std::vector<size_t> ids;
    ids.reserve(result.size());

    for (const auto& [k, v] : result)
        ids.push_back(v);

    return ids;
}

inline std::unordered_set<size_t> to_set(const std::vector<size_t>& ids)
{
    std::unordered_set<size_t> s;
    s.reserve(ids.size());
    for (auto id : ids) s.insert(id);
    return s;
}

} // namespace triag_core