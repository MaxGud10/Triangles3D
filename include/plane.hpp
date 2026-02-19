#pragma once

#include <cmath>
#include <limits>

#include "config.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace triangle
{

template <typename PointTy = double>
class Plane
{
  static constexpr PointTy NaN = std::numeric_limits<PointTy>::quiet_NaN();

  PointTy A = NaN; // Ax + By + Cz + D = 0.
  PointTy B = NaN;
  PointTy C = NaN;
  PointTy D = NaN;

  PointTy normal_length = NaN;
  Vector<PointTy> normal;

  void normalize_plane()
  {
    normal_length = std::sqrt(A * A + B * B + C * C);

    A = A / normal_length;
    B = B / normal_length;
    C = C / normal_length;
    D = D / normal_length;
  }

public:
  Plane(const Point<PointTy> &a, const Point<PointTy> &b,
        const Point<PointTy> &c)
  {
    A = (b.y - a.y) * (c.z - a.z) -
        (b.z - a.z) * (c.y - a.y);
    B = (b.z - a.z) * (c.x - a.x) -
        (b.x - a.x) * (c.z - a.z);
    C = (b.x - a.x) * (c.y - a.y) -
        (b.y - a.y) * (c.x - a.x);

    D = -(A * a.x + B * a.y + C * a.z);

    normal.x = A;
    normal.y = B;
    normal.z = C;

    normalize_plane(); // ‖A,B,C‖ = 1
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
    std::cout << A << "x + " << B << "y + " << C << "z + " << D << " = 0" << std::endl;
  }

  void print_normal() const
  {
    std::cout << "(" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
  }

  PointTy substitute(const Point<PointTy> &point) const
  {
    return A * point.x + B * point.y + C * point.z + D;
  }

  PointTy substitute(const PointTy &x1, const PointTy &y1, const PointTy &z1) const
  {
    return A * x1 + B * y1 + C * z1 + D;
  }

  // double_cmp() is used for comparison with epsilon to account for rounding errors
  bool almost_equal(const Plane<PointTy> &other) const
  {
    // we will check for plane coincidence with accuracy to the sign
    if ((double_cmp( A, other.A) && double_cmp( B, other.B) &&
         double_cmp( C, other.C) && double_cmp( D, other.D)) ||
        (double_cmp(-A, other.A) && double_cmp(-B, other.B) &&
         double_cmp(-C, other.C) && double_cmp(-D, other.D)))
    {
      return true;
    }

    return false;
  }

  bool operator==(const Plane<PointTy> &other) const = default;


  PointTy get_A() const { return A; }
  PointTy get_B() const { return B; }
  PointTy get_C() const { return C; }
  PointTy get_D() const { return D; }

  Vector<PointTy> get_normal() const { return normal; }

  bool planes_are_parallel(const Plane<PointTy> &other) const
  {
    if ((double_cmp( this->get_A(), other.get_A()) &&
         double_cmp( this->get_B(), other.get_B()) &&
         double_cmp( this->get_C(), other.get_C())) ||
        (double_cmp(-this->get_A(), other.get_A()) &&
         double_cmp(-this->get_B(), other.get_B()) &&
         double_cmp(-this->get_C(), other.get_C())))
    {
      return true; // A1,B1,C1) ~= +-(A2,B2,C2)
    }

    return false;
  }

  // direction of the intersection line of two non-parallel planes
  Vector<PointTy> get_planes_intersection_vector(const Plane<PointTy> &other) const
  {
    Vector<PointTy> vec = cross(this->get_normal(), other.get_normal());

    return vec;
  }

  Point<PointTy> get_planes_intersection_point(const Plane<PointTy> &other) const
  {
    static constexpr PointTy NaN = std::numeric_limits<PointTy>::quiet_NaN();

    PointTy x = NaN;
    PointTy y = NaN;
    PointTy z = NaN;

    PointTy determinant_x_zero = this->get_B() * other.get_C() - this->get_C() * other.get_B();
    PointTy determinant_y_zero = this->get_A() * other.get_C() - this->get_C() * other.get_A();
    PointTy determinant_z_zero = this->get_A() * other.get_B() - this->get_B() * other.get_A();

    // A1x + B1y = -D1
    // A2x + B2y = -D2
    if (!double_cmp(determinant_z_zero, PointTy{0}))
    {
      x = (this->get_B() * other.get_D() - other.get_B() * this->get_D()) / determinant_z_zero;
      y = (other.get_A() * this->get_D() - this->get_A() * other.get_D()) / determinant_z_zero;
      z = PointTy{0};
    }

    // A1x + С1z = -D1
    // A2x + С2z = -D2
    else if (!double_cmp(determinant_y_zero, PointTy{0}))
    {
      x = (this->get_C() * other.get_D() - other.get_C() * this->get_D()) / determinant_y_zero;
      z = (other.get_A() * this->get_D() - this->get_A() * other.get_D()) / determinant_y_zero;
      y = PointTy{0};
    }

    // B1y + С1z = -D1
    // B2y + С2z = -D2
    else if (!double_cmp(determinant_x_zero, PointTy{0}))
    {
      y = (this->get_C() * other.get_D() - other.get_C() * this->get_D()) / determinant_x_zero;
      z = (other.get_B() * this->get_D() - this->get_B() * other.get_D()) / determinant_x_zero;
      x = PointTy{0};
    }

    Point<PointTy> intersectionPoint = {x, y, z};

    return intersectionPoint;
  }
};
} // namespace triangle