#include "../include/triangles.hpp"

int main() 
{
  size_t TriagNum = 0;
  std::cin >> TriagNum;

  std::vector<Triangle<double>> input;

  for (size_t i = 0; i < TriagNum; ++i) 
  {
    double x1 = 0, y1 = 0, z1 = 0;
    double x2 = 0, y2 = 0, z2 = 0;
    double x3 = 0, y3 = 0, z3 = 0;

    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

    Triangle<double> triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    input.push_back(triangle);
  }

  for (size_t i = 0; i < TriagNum - 1; ++i) 
  {
    for (size_t j = i + 1; j < TriagNum; ++j) 
    {
      if (check_intersection(input[i], input[j]))
        std::cout << "Intersect\n";
    }
  }
}