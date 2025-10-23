#include "../include/config.hpp"

bool   double_cmp     (double x, double y)                     { return fabs(x - y) <= _epsilon; }
double get_min_of_four(double a, double b, double c, double d) { return std::min(a, std::min(b, std::min(c, d))); }
double get_max_of_four(double a, double b, double c, double d) { return std::max(a, std::max(b, std::max(c, d))); }