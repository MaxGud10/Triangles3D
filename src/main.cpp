#include "octotree.hpp"
#include "triangle_with_id.hpp"
#include "intersection.hpp"

int main() 
{
  using PointTy = double;

  std::vector<triangle::ShapeWithId<PointTy>> input;
  size_t triag_num = 0;
  std::cin >> triag_num;
  input.reserve(triag_num);

  for (size_t i = 0; i < triag_num; ++i) 
  {
    PointTy x1 = 0, y1 = 0, z1 = 0;
    PointTy x2 = 0, y2 = 0, z2 = 0;
    PointTy x3 = 0, y3 = 0, z3 = 0;

    if (!(std::cin >> x1 >> y1 >> z1
                   >> x2 >> y2 >> z2
                   >> x3 >> y3 >> z3)) 
    {
      std::cerr << "Input error: see README for correct usage";

      return 1;
    }

    triangle::ShapeWithId<PointTy> rec;

    triangle::Point<PointTy> a{x1, y1, z1};
    triangle::Point<PointTy> b{x2, y2, z2};
    triangle::Point<PointTy> c{x3, y3, z3};

    rec.shape = triangle::make_shape_from_points<PointTy>(a, b, c);

    rec.min_bound = {std::min({a.x, b.x, c.x}),
                  std::min({a.y, b.y, c.y}),
                  std::min({a.z, b.z, c.z})};

    rec.max_bound = {std::max({a.x, b.x, c.x}),
                  std::max({a.y, b.y, c.y}),
                  std::max({a.z, b.z, c.z})};

    rec.id = i;

    input.push_back(rec);
  }

  triangle::Octotree<PointTy> octotree(input);
  octotree.divide_tree();

  std::map<size_t, size_t>                   result;
  std::deque<triangle::BoundingBox<PointTy>> octotree_cells = octotree.get_cells();


  for (auto it : octotree_cells) 
  {
    it.group_intersections(result);
  }

  for (auto it = result.begin(); it != result.end(); ++it) 
  {
    std::cout << it->second << std::endl;
  }
}