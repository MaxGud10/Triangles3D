#pragma once

#include <deque>
#include <map>
#include <vector>
#include <list>

#include "triangle_with_id.hpp"
#include "intersection.hpp"

namespace triangle
{


template <typename PointTy = double>
class BoundingBox
{
  std::deque<ShapeWithId<PointTy>> incell;

  Vector<PointTy> min;
  Vector<PointTy> max;

public:
  template <typename Iter>
  BoundingBox(Iter first, Iter last)
  {
    incell.insert(incell.end(), first, last);

    if (incell.empty())
            return;

    auto it = incell.begin();

    min.x = max.x = it->min_x();
    min.y = max.y = it->min_y();
    min.z = max.z = it->min_z();

    for (; it != incell.end(); ++it)
    {
      max.x = std::max(max.x, it->max_x());
      min.x = std::min(min.x, it->min_x());

      max.y = std::max(max.y, it->max_y());
      min.y = std::min(min.y, it->min_y());

      max.z = std::max(max.z, it->max_z());
      min.z = std::min(min.z, it->min_z());
    }
  }

  PointTy average_x() const { return (min.x + max.x) / static_cast<PointTy>(2); }
  PointTy average_y() const { return (min.y + max.y) / static_cast<PointTy>(2); }
  PointTy average_z() const { return (min.z + max.z) / static_cast<PointTy>(2); }

        std::deque<ShapeWithId<PointTy>> &get_incell()       { return incell; }
  const std::deque<ShapeWithId<PointTy>> &get_incell() const { return incell; }

  void group_intersections(std::map<size_t, size_t> &result)
  {
    for (auto one = incell.begin(); one != incell.end(); ++one)
    {
      auto it = one;
      it++;

      for (auto two = it; two != incell.end(); ++two)
      {
        if (check_intersection(one->shape, two->shape))
        {
            result[one->id] = one->id;
            result[two->id] = two->id;
        }
      }
    }
  }
};

template <typename PointTy = double>
class Octotree
{
  std::vector<ShapeWithId<PointTy>> input;
  std::deque <BoundingBox<PointTy>> cells;

  size_t depth     = 0;
  size_t cells_num = 0;
  size_t axis      = 0;
  size_t axes      = 3;

  static constexpr size_t max_depth        = 3;

  static constexpr size_t depth0_threshold = 1000;
  static constexpr size_t depth1_threshold = 10000;
  static constexpr size_t depth2_threshold = 100000;

public:
  Octotree(const std::vector<ShapeWithId<PointTy>> &shapes) : input(shapes)
  {
      cells.emplace_back (input.begin(), input.end());
      depth = count_depth(input.size());

      ++cells_num;
  }

  size_t count_depth(const size_t &triag_num) const
  {
    if (triag_num < depth0_threshold)
      return 0;

    if (triag_num < depth1_threshold)
      return 1;

    if (triag_num < depth2_threshold)
      return 2;

    return max_depth;
  }


  const std::deque<BoundingBox<PointTy>> &get_cells() { return cells; }

  void divide_cell()
  {
    std::vector<ShapeWithId<PointTy>> plus;
    std::vector<ShapeWithId<PointTy>> minus;

    size_t copy_num_of_cells = cells_num;

    for (int i = 0; i < copy_num_of_cells; ++i)
    {
      auto front_groups = cells.front();

      size_t  axis    = axis % 3;
      PointTy average = calculate_average(front_groups, axis);

      plus .reserve(front_groups.get_incell().size());
      minus.reserve(front_groups.get_incell().size());

      for (const auto& it : front_groups.get_incell())
      {
          PointTy min_on_axis = PointTy{0};
          PointTy max_on_axis = PointTy{0};

          switch (axis)
          {
            case 0: min_on_axis = it.min_x(); max_on_axis = it.max_x(); break;
            case 1: min_on_axis = it.min_y(); max_on_axis = it.max_y(); break;
            case 2: min_on_axis = it.min_z(); max_on_axis = it.max_z(); break;

            default: throw std::invalid_argument("invalid axis");
          }

          if (max_on_axis >= average)
            plus.push_back(it);

          if (min_on_axis <= average)
            minus.push_back(it);
      }

      if (plus.size() + minus.size() < front_groups.get_incell().size() * 2)
      {
        if (!plus.empty())
        {
          cells.emplace_back(plus.begin(), plus.end());

          ++cells_num;
        }

        if (!minus.empty())
        {
          cells.emplace_back(minus.begin(), minus.end());

          ++cells_num;
        }

        cells.pop_front();

        --cells_num;
      }

      plus .clear();
      minus.clear();

      ++axis;
    }
  }

  PointTy calculate_average(const BoundingBox<PointTy> &box, int axis) const
  {
    switch (axis)
    {
      case 0:
        return box.average_x();

      case 1:
        return box.average_y();

      case 2:
        return box.average_z();

      default:
        throw std::invalid_argument("Invalid axis");
    }
  }

  void divide_tree()
  {
    for (int i = 0; i < depth; ++i)
    {
      for (int i = 0; i < axes; ++i)
        divide_cell();
    }
  }
};

} // namespace triangle