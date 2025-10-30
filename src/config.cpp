#include "../include/config.hpp"

namespace triangle 
{

bool double_cmp(double x, double y) { return std::fabs(x - y) <= _epsilon; }

} // namespace triangle
