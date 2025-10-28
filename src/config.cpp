#include "../include/config.hpp"

bool double_cmp(double x, double y) { return fabs(x - y) <= _epsilon; }


bool g_debug = false;