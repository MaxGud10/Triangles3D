#pragma once

#include "vector.hpp"

namespace triangle 
{

template <typename PointTy = double> 
class Plane 
{
  PointTy A, B, C, D;
  PointTy normal_length;
  Vector<PointTy> normal;

  void normalize_plane() 
  {
    normal_length = std::sqrt(A * A + B * B + C * C);

    if (cmp(normal_length, 0.0))
      return;

    A = A / normal_length;
    B = B / normal_length;
    C = C / normal_length;
    D = D / normal_length;
  }

public:
    Plane(const Point<PointTy> &a, const Point<PointTy> &b, const Point<PointTy> &c) 
    {
        A = (b.y - a.y) * (c.z - a.z) - (b.z - a.z) * (c.y - a.y);
        B = (b.z - a.z) * (c.x - a.x) - (b.x - a.x) * (c.z - a.z);
        C = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        D = -(A * a.x + B * a.y + C * a.z);

        normal.x = A;
        normal.y = B;
        normal.z = C;

        normalize_plane();
    }

    Plane(const PointTy &x1, const PointTy &y1, const PointTy &z1,
          const PointTy &x2, const PointTy &y2, const PointTy &z2,
          const PointTy &x3, const PointTy &y3, const PointTy &z3) 
    {
        A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
        B = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
        C = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
        D = -(A * x1 + B * y1 + C * z1);

        normal.x = A;
        normal.y = B;
        normal.z = C;

        normalize_plane();
    }

    void print() const 
    {
        std::cout << A << "x + " << B << "y + " << C << "z + " << D << " = 0"
                  << std::endl;
    }

    PointTy substitute(const Point<PointTy> &point) const 
    {
        return A * point.x + B * point.y + C * point.z + D;
    }

    bool operator==(const Plane<PointTy> &other) const 
    {
        // Checking for alignment of planes with precision to the sign
        if ((cmp( A, other.A) && cmp(B, other.B) && cmp(C, other.C) &&
             cmp( D, other.D)) ||
            (cmp(-A, other.A) && cmp(-B, other.B) && cmp(-C, other.C) &&
             cmp(-D, other.D))) 
        {
            return true;
        }

        return false;
    }

    PointTy get_A() const { return A; }
    PointTy get_B() const { return B; }
    PointTy get_C() const { return C; }
    PointTy get_D() const { return D; }

    Vector<PointTy> get_normal() const { return normal; }
};

template <typename PointTy = double>
bool planes_are_parallel(const Plane<PointTy> &plane1,
                         const Plane<PointTy> &plane2) 
{
    if ((cmp( plane1.get_A(), plane2.get_A()) &&
         cmp( plane1.get_B(), plane2.get_B()) &&
         cmp( plane1.get_C(), plane2.get_C())) ||
        (cmp(-plane1.get_A(), plane2.get_A()) &&
         cmp(-plane1.get_B(), plane2.get_B()) &&
         cmp(-plane1.get_C(), plane2.get_C()))) 
    {
        return true;
    }

    return false;
}

template <typename PointTy = double>
Vector<PointTy> get_planes_intersection_vector(const Plane<PointTy> &plane1,
                                               const Plane<PointTy> &plane2) 
{
    return cross(plane1.get_normal(), plane2.get_normal());
}

template <typename PointTy = double>
Point<PointTy> get_planes_intersection_point(const Plane<PointTy> &p1,
                                             const Plane<PointTy> &p2) 
{
    PointTy x, y, z;

    PointTy det_x_zero = p1.get_B() * p2.get_C() - p1.get_C() * p2.get_B();
    PointTy det_y_zero = p1.get_A() * p2.get_C() - p1.get_C() * p2.get_A();
    PointTy det_z_zero = p1.get_A() * p2.get_B() - p1.get_B() * p2.get_A();

    // A1x + B1y = -D1
    // A2x + B2y = -D2
    if (!cmp(det_z_zero, 0.0)) 
    {
        x = (p1.get_B() * p2.get_D() - p2.get_B() * p1.get_D()) / det_z_zero;
        y = (p2.get_A() * p1.get_D() - p1.get_A() * p2.get_D()) / det_z_zero;
        z = 0.0;
    }
    // A1x + С1z = -D1
    // A2x + С2z = -D2
    else if (!cmp(det_y_zero, 0.0)) 
    {
        x = (p1.get_C() * p2.get_D() - p2.get_C() * p1.get_D()) / det_y_zero;
        z = (p2.get_A() * p1.get_D() - p1.get_A() * p2.get_D()) / det_y_zero;
        y = 0.0;
    }
    // B1y + С1z = -D1
    // B2y + С2z = -D2
    else if (!cmp(det_x_zero, 0.0)) 
    {
        y = (p1.get_C() * p2.get_D() - p2.get_C() * p1.get_D()) / det_x_zero;
        z = (p2.get_B() * p1.get_D() - p1.get_B() * p2.get_D()) / det_x_zero;
        x = 0.0;
    }

    Point<PointTy> intersectionPoint{x, y, z};
    return intersectionPoint;
}
} // namespace triangle