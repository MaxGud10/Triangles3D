#pragma once

#include <deque>
#include <list>
#include <map>

#include "triangles.hpp"


template <typename PointTy = double> 
class BoundingBox 
{
  std::list<Triangle<PointTy>> incell;

  Vector<PointTy> min;
  Vector<PointTy> max;

public:
  BoundingBox(const std::list<Triangle<PointTy>> &triangles) : incell(triangles) 
  {
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

  PointTy average_x() const { return (max.x + min.x) / 2; }
  PointTy average_y() const { return (max.y + min.y) / 2; }
  PointTy average_z() const { return (max.z + min.z) / 2; }

  std::list<Triangle<PointTy>> &get_incell() { return incell; }

  void group_intersections(std::map<size_t, size_t> &result) 
  {
    for (auto one = incell.begin(); one != incell.end(); ++one) 
    {
      auto it = one;
      it++;

      for (auto two = it; two != incell.end(); ++two) 
      {
        if (check_intersection(*one, *two)) 
        {
          result[(*one).id] = (*one).id;
          result[(*two).id] = (*two).id;
        }
      }
    }
  }
};

template <typename PointTy = double> 
class Octotree 
{
  std::list<Triangle<PointTy>>     input;
  std::deque<BoundingBox<PointTy>> cells;

  size_t depth     = 0;
  size_t cells_num = 0;
  size_t triag_num = 0;

public:
  Octotree(const std::list<Triangle<PointTy>> &triangles, const size_t triag_num) : input(triangles), triag_num(triag_num) 
  {
    cells.push_back(BoundingBox<PointTy>(input));
    depth = count_depth(triag_num);

    ++cells_num;
  };

  size_t count_depth(const size_t &triag_num) const 
  {
    if (triag_num < 1000)
      return 0;

    if (triag_num < 10000)
      return 1;

    if (triag_num < 100000)
      return 2;

    return 3;
  }


  const std::deque<BoundingBox<PointTy>> &get_cells() { return cells; }


  void divide_cell() 
  {
    static int axis = 0;

    std::list<Triangle<PointTy>> plus;
    std::list<Triangle<PointTy>> minus;

    size_t copy_num_of_cells = cells_num;

    for (int i = 0; i < copy_num_of_cells; ++i) 
    {
      auto front_groups = cells.front();

      size_t  nod     = axis % 3;
      PointTy average = calculate_average(front_groups, nod);

      for (const auto &it : front_groups.get_incell()) 
      {
        PointTy coordinates[3][3] = 
        {
            it.get_a().get_x(), it.get_b().get_x(), it.get_c().get_x(),
            it.get_a().get_y(), it.get_b().get_y(), it.get_c().get_y(),
            it.get_a().get_z(), it.get_b().get_z(), it.get_c().get_z()};

        if (coordinates[nod][0] >= average || coordinates[nod][1] >= average ||
            coordinates[nod][2] >= average)
        {
          plus.push_back(it);
        }

        if (coordinates[nod][0] <= average || coordinates[nod][1] <= average ||
            coordinates[nod][2] <= average) 
        {
          minus.push_back(it);
        }
      }

      if (plus.size() + minus.size() < front_groups.get_incell().size() * 2) 
      {
        if (!plus.empty()) 
        {
          cells.push_back(BoundingBox<PointTy>(plus));

          ++cells_num;
        }

        if (!minus.empty()) 
        {
          cells.push_back(BoundingBox<PointTy>(minus));

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

  PointTy calculate_average(const BoundingBox<PointTy> &box, int nod) const 
  {
    switch (nod) 
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
      for (int i = 0; i < 3; ++i)
        divide_cell();
    }
  }
};