#include "../include/octotree.hpp"
#include "../include/triangles.hpp"

int main() 
{
  std::list<Triangle<double>> input;
  size_t                      TriagNum = 0;

  std::cin >> TriagNum;

  double min_x = -1000000;
  double min_y = -1000000;
  double min_z = -1000000;

  double max_x =  1000000;
  double max_y =  1000000;
  double max_z =  1000000;

  for (size_t i = 0; i < TriagNum; ++i) 
  {
    double x1 = 0, y1 = 0, z1 = 0;
    double x2 = 0, y2 = 0, z2 = 0;
    double x3 = 0, y3 = 0, z3 = 0;

    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

    min_x = get_min_of_four(min_x, x1, x2, x3);
    min_y = get_min_of_four(min_y, y1, y2, y3);
    min_z = get_min_of_four(min_z, z1, z2, z3);

    max_x = get_max_of_four(min_x, x1, x2, x3);
    max_y = get_max_of_four(min_y, y1, y2, y3);
    max_z = get_max_of_four(min_z, z1, z2, z3);

    Triangle<double> triangle(x1, y1, z1, 
                              x2, y2, z2, 
                              x3, y3, z3);
    triangle.id = i;
    input.push_back(triangle);
  }

  BoundingBox<double> BB(Vector<double>(min_x, min_y, min_z),
                         Vector<double>(max_x, max_y, max_z));

  OctTree<double> tree(BB, input);
  tree.BuildTree();

  std::list<int> res = tree.getAllTreeIntersectNum();
  for (auto elem : res) 
  {
    std::cout << elem << '\n';
  }
}