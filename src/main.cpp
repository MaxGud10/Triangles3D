#include "../include/triangles.hpp"
#include "list"

int main() 
{
    size_t TriagNum = 0;
    std::cin >> TriagNum;

    std::list<Triangle<float>> input;

    for (size_t i = 0; i < TriagNum; ++i) 
    {
        float x1 = 0, y1 = 0, z1 = 0;
        float x2 = 0, y2 = 0, z2 = 0;
        float x3 = 0, y3 = 0, z3 = 0;

        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

        Triangle<float> triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
        input.push_back(triangle);
        triangle.print();
        std::cout << triangle.type << "\n\n";
    }
}