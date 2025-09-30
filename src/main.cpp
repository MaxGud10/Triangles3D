#include "../include/triangles.hpp"
#include <unordered_set>

int main()
{
  size_t N = 0;
  std::cin >> N;

  std::vector<Triangle<double>> tris;
  tris.reserve(N);

  for (size_t i = 0; i < N; ++i) 
  {
    double x1 = 0, y1 = 0, z1 = 0;
    double x2 = 0, y2 = 0, z2 = 0;
    double x3 = 0, y3 = 0, z3 = 0;

    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
    tris.emplace_back(x1,y1,z1, x2,y2,z2, x3,y3,z3);
  }

  std::vector<char> hit(N, 0);

  for (size_t i = 0; i + 1 < N; ++i) 
  {
    for (size_t j = i + 1; j < N; ++j) 
    {
      if (check_intersection(tris[i], tris[j])) 
      {
        hit[i] = 1;
        hit[j] = 1;
      }
    }
  }

  for (size_t i = 0; i < N; ++i) 
  {
    if (hit[i]) std::cout << i << "\n";
  }
}
