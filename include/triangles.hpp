#pragma once

#include <array>

#include "interval.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace triangle
{

template <typename PointTy = double>
class Triangle
{
public:
  Point<PointTy> a, b, c;

private:
  PointTy coord(const Point<PointTy> &p, int axis) const
  {
      switch (axis)
      {
        case 0: return p.x;
        case 1: return p.y;
        case 2: return p.z;

        default: throw std::invalid_argument("Invalid axis");
      }
  }

  PointTy min_coord(int axis) const { return std::min({coord(a, axis), coord(b, axis), coord(c, axis)}); }
  PointTy max_coord(int axis) const { return std::max({coord(a, axis), coord(b, axis), coord(c, axis)}); }


  bool point_is_coplanar(const Point<PointTy> &point) const
  {
      // checking the coplanarity
      Plane<PointTy> pl(get_a(), get_b(), get_c());
      const auto sd = pl.substitute(point);

      return double_cmp(sd, PointTy{0});
  }

  bool same_point(const Point<PointTy> &U, const Point<PointTy> &V) const
  {
      return double_cmp(U.x, V.x) &&
             double_cmp(U.y, V.y) &&
             double_cmp(U.z, V.z);
  }

  bool point_on_segment(const Point<PointTy> &X, const Point<PointTy> &Y, const Point<PointTy> &point) const
  {
      const Vector<PointTy> XY = vector_from_point(Y     - X);
      const Vector<PointTy> XP = vector_from_point(point - X);

      const auto cr = cross(XY, XP);

      if (!double_cmp(cr.x, PointTy{0}) ||
          !double_cmp(cr.y, PointTy{0}) ||
          !double_cmp(cr.z, PointTy{0}))
          return false;

      const PointTy minx = std::min(X.x, Y.x) - epsilon;
      const PointTy maxx = std::max(X.x, Y.x) + epsilon;
      const PointTy miny = std::min(X.y, Y.y) - epsilon;
      const PointTy maxy = std::max(X.y, Y.y) + epsilon;
      const PointTy minz = std::min(X.z, Y.z) - epsilon;
      const PointTy maxz = std::max(X.z, Y.z) + epsilon;

      return (point.x >= minx && point.x <= maxx &&
              point.y >= miny && point.y <= maxy &&
              point.z >= minz && point.z <= maxz);
  }

  bool point_in_degenerate_triangle(const Point<PointTy> &point) const
  {
      const auto &A = get_a();
      const auto &B = get_b();
      const auto &C = get_c();

      return point_on_segment(A, B, point) || point_on_segment(A, C, point) || point_on_segment(B, C, point) ||
             same_point(point, A)          || same_point(point, B)          || same_point(point, C);
  }

  bool point_in_non_degenerate_triangle(const Point<PointTy> &point) const
  {
      const Vector<PointTy> v0 = vector_from_point(get_c() - get_a()); // C - A
      const Vector<PointTy> v1 = vector_from_point(get_b() - get_a()); // B - A
      const Vector<PointTy> v2 = vector_from_point(point   - get_a()); // point - A

      const PointTy dot00 = dot(v0, v0);
      const PointTy dot01 = dot(v0, v1);
      const PointTy dot02 = dot(v0, v2);
      const PointTy dot11 = dot(v1, v1);
      const PointTy dot12 = dot(v1, v2);

      const PointTy denom = dot00 * dot11 - dot01 * dot01;
      if (double_cmp(denom, PointTy{0}))
          return false;

      const PointTy inv = PointTy{1} / denom;
      const PointTy u   = (dot11 * dot02 - dot01 * dot12) * inv;
      const PointTy v   = (dot00 * dot12 - dot01 * dot02) * inv;

      const PointTy eps = PointTy(epsilon) * PointTy{10};

      if (u >= -eps && v >= -eps && (u + v) <= (PointTy{1} + eps))
          return true;

      return (double_cmp(u,     PointTy{0}) || u > 0) &&
             (double_cmp(v,     PointTy{0}) || v > 0) &&
             (double_cmp(u + v, PointTy{1}) || (u + v) < PointTy{1} + eps);
  }


public:
  Triangle() = default;

  Triangle(const Point<PointTy> &p1, const Point<PointTy> &p2, const Point<PointTy> &p3)
                             : a(p1),                    b(p2),                    c(p3)
  {
    if (a.norm() <= b.norm()) std::swap(a, b);

    if (a.norm() <= c.norm()) std::swap(a, c);

    if (b.norm() <= c.norm()) std::swap(b, c);
  }

  void print() const
  {
    a.print();
    b.print();
    c.print();
  }

  const Point<PointTy> &get_a() const { return a; }
  const Point<PointTy> &get_b() const { return b; }
  const Point<PointTy> &get_c() const { return c; }

  PointTy min_x() const { return min_coord(0); }
  PointTy max_x() const { return max_coord(0); }
  PointTy min_y() const { return min_coord(1); }
  PointTy max_y() const { return max_coord(1); }
  PointTy min_z() const { return min_coord(2); }
  PointTy max_z() const { return max_coord(2); }

  std::pair<Point<PointTy>, Point<PointTy>> get_triangle_space() const
  {
    return {Point<PointTy>{min_x(), min_y(), min_z()},
            Point<PointTy>{max_x(), max_y(), max_z()}};
  }

  // is the point inside the triangle
  bool is_point_in_triangle(const Point<PointTy> &point) const
  {
      if (!point_is_coplanar(point))
          return false;

      // barycentric coordinates
      const Vector<PointTy> v0 = vector_from_point(get_c() - get_a());
      const Vector<PointTy> v1 = vector_from_point(get_b() - get_a());

      const PointTy dot00 = dot(v0, v0);
      const PointTy dot01 = dot(v0, v1);
      const PointTy dot11 = dot(v1, v1);

      const PointTy denom = dot00 * dot11 - dot01 * dot01;

      // for a degenerate triangle: check if it hits one of the edges / vertices
      if (double_cmp(denom, PointTy{0}))
          return point_in_degenerate_triangle(point);

      return point_in_non_degenerate_triangle(point);
  }


};

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_3D(Triangle<PointTy> &t1, Triangle<PointTy> &t2)
{
  Plane<PointTy> plane1(t1.get_a(), t1.get_b(), t1.get_c());
  Plane<PointTy> plane2(t2.get_a(), t2.get_b(), t2.get_c());

  // let's check the planes for a match
  if (plane1.planes_are_parallel(plane2))
  {
    // are they collinear
    if (double_cmp( plane1.get_D(), plane2.get_D()) ||
        double_cmp(-plane1.get_D(), plane2.get_D()))
    {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }
    return false;
  }

  // distances of vertices to planes
  auto same_strict_sign = [](PointTy x, PointTy y, PointTy z)
  {
    auto pos = [](PointTy v) { return v > PointTy{0} && !double_cmp(v, PointTy{0}); };
    auto neg = [](PointTy v) { return v < PointTy{0} && !double_cmp(v, PointTy{0}); };

    bool all_pos = pos(x) && pos(y) && pos(z);
    bool all_neg = neg(x) && neg(y) && neg(z);

    return all_pos || all_neg;
  };

  // if all the signed distances from the vertices of T2 to the triangle T1 are of the same sign,
  // then the triangles do not intersect
  PointTy signed_dist11 = plane1.substitute(t2.get_a());
  PointTy signed_dist21 = plane1.substitute(t2.get_b());
  PointTy signed_dist31 = plane1.substitute(t2.get_c());
  if (same_strict_sign(signed_dist11, signed_dist21, signed_dist31))
    return false;

  // if all the signed distances from the vertices of T1 to triangle T2 are of the same sign,
  // then the triangles do not intersect
  PointTy signed_dist12 = plane2.substitute(t1.get_a());
  PointTy signed_dist22 = plane2.substitute(t1.get_b());
  PointTy signed_dist32 = plane2.substitute(t1.get_c());
  if (same_strict_sign(signed_dist12, signed_dist22, signed_dist32))
    return false;


  // then we will check for intersection. we will find the intersection line of two planes
  Line<PointTy> inter_line{plane1.get_planes_intersection_vector(plane2),
                           plane1.get_planes_intersection_point (plane2)};

  // find the intersection intervals of the triangles with the intersection line of the planes
  auto interval1 = get_interval_by_plane_clip(t1, plane2); // plane T2 cuts triangle T1
  auto interval2 = get_interval_by_plane_clip(t2, plane1); // plane T1 cuts triangle T2
  if (!interval1.valid() || !interval2.valid())
  {
    return false;
  }

  // let's check if the intervals intersect.
  return interval1.intersect_intervals(interval2);
}

template <typename PointTy = double>
Interval<PointTy> get_interval_of_triangle_and_line(const Line<PointTy> &inter_line, const Triangle<PointTy> &triangle)
{
  Line<PointTy> line1{vector_from_point(triangle.get_b() - triangle.get_a()), triangle.get_a()};
  Line<PointTy> line2{vector_from_point(triangle.get_c() - triangle.get_a()), triangle.get_a()};
  Line<PointTy> line3{vector_from_point(triangle.get_c() - triangle.get_b()), triangle.get_b()};

  Point<PointTy> inter_point1 = intersect_line_with_line(inter_line, line1);
  Point<PointTy> inter_point2 = intersect_line_with_line(inter_line, line2);
  Point<PointTy> inter_point3 = intersect_line_with_line(inter_line, line3);

  Interval interval = get_valid_interval_of_triangle_and_line(triangle, inter_point1, inter_point2, inter_point3);

  return interval;
}

template <typename PointTy>
int get_excluded_axis_index_for_2D(const Plane<PointTy> &plane)
{
  int excluded_axis_index = 2;

  PointTy absA = std::abs(plane.get_A()); // |A|
  PointTy absB = std::abs(plane.get_B()); // |B|
  PointTy absC = std::abs(plane.get_C()); // |C|

  // the largest absolute value of the normal component indicates the axis that is most advantageous to exclude
  if (absA >= absB && absA >= absC)
    excluded_axis_index = 0; // working with pairs (y,z)

  else if (absB >= absA && absB >= absC)
    excluded_axis_index = 1; // working with pairs (x,z)

  else
    excluded_axis_index = 2; // working with pairs (x,y)

  return excluded_axis_index;
}

template <typename PointTy>
std::pair<PointTy, PointTy> project_point_to_2d(const Point<PointTy> &p, int excluded_axis_index)
{
  if (excluded_axis_index == 0)  return { p.y, p.z }; // (y, z)
  if (excluded_axis_index == 1)  return { p.x, p.z }; // (x, z)
  /* excluded_axis_index == 2 */ return { p.x, p.y }; // (x, y)
}

// orientation of the square in 2D
template <typename PointTy>
PointTy oriented_area_2d(PointTy ax, PointTy ay,
                         PointTy bx, PointTy by,
                         PointTy cx, PointTy cy)
{
  // >0 — to the left of directed AB; <0 — to the right; =0 — collinear
  return (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
}

// let's check if our point P lies on the segment AB
template <typename PointTy>
bool point_lies_on_segment_2d(PointTy ax, PointTy ay,
                              PointTy bx, PointTy by,
                              PointTy px, PointTy py)
{
  PointTy area = (bx-ax)*(py-ay) - (by-ay)*(px-ax);
  if (!double_cmp(area, PointTy{0}))
    return false; // not collinear

  PointTy minx = std::min(ax,bx), maxx = std::max(ax,bx);
  PointTy miny = std::min(ay,by), maxy = std::max(ay,by);

  return (px >= minx && px <= maxx && py >= miny && py <= maxy);
}

// intersection of two segments AB and CD in 2D
template <typename PointTy>
bool segments_intersect_2d(PointTy ax, PointTy ay,
                           PointTy bx, PointTy by,
                           PointTy cx, PointTy cy,
                           PointTy dx, PointTy dy)
{
  PointTy o1 = oriented_area_2d<PointTy>(ax,ay,bx,by,cx,cy);
  PointTy o2 = oriented_area_2d<PointTy>(ax,ay,bx,by,dx,dy);
  PointTy o3 = oriented_area_2d<PointTy>(cx,cy,dx,dy,ax,ay);
  PointTy o4 = oriented_area_2d<PointTy>(cx,cy,dx,dy,bx,by);

  const PointTy zero{0};
  bool sep1 = (o1 > zero && o2 < zero) || (o1 < zero && o2 > zero);
  bool sep2 = (o3 > zero && o4 < zero) || (o3 < zero && o4 > zero);

  if (sep1 && sep2)
    return true; // the segments intersect in a common position

  // special cases: touching/matching - by checking the endpoints' belonging to segments
  if (point_lies_on_segment_2d<PointTy>(ax,ay,bx,by,cx,cy)) return true;
  if (point_lies_on_segment_2d<PointTy>(ax,ay,bx,by,dx,dy)) return true;
  if (point_lies_on_segment_2d<PointTy>(cx,cy,dx,dy,ax,ay)) return true;
  if (point_lies_on_segment_2d<PointTy>(cx,cy,dx,dy,bx,by)) return true;

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_2D(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2)
{
  Plane<PointTy> plane(t1.get_a(), t1.get_b(), t1.get_c()); // we built the plane by t1

  // let's choose which coordinate to exclude in order to work in two-dimensional coordinates
  int excluded_axis_index = get_excluded_axis_index_for_2D<PointTy>(plane);

  // we project the vertices of both triangles into 2D
  std::pair<PointTy,PointTy> a1 = project_point_to_2d<PointTy>(t1.get_a(), excluded_axis_index);
  std::pair<PointTy,PointTy> b1 = project_point_to_2d<PointTy>(t1.get_b(), excluded_axis_index);
  std::pair<PointTy,PointTy> c1 = project_point_to_2d<PointTy>(t1.get_c(), excluded_axis_index);

  std::pair<PointTy,PointTy> a2 = project_point_to_2d<PointTy>(t2.get_a(), excluded_axis_index);
  std::pair<PointTy,PointTy> b2 = project_point_to_2d<PointTy>(t2.get_b(), excluded_axis_index);
  std::pair<PointTy,PointTy> c2 = project_point_to_2d<PointTy>(t2.get_c(), excluded_axis_index);

  // check that point P=(px,py) is inside/on the boundary of triangle ABC in 2D
  // that is, if all the oriented areas relative to the edges of ABC have the same sign or 0
  auto point_inside_triangle_2d = [&](PointTy px, PointTy py,
                                      const std::pair<PointTy, PointTy>& A,
                                      const std::pair<PointTy, PointTy>& B,
                                      const std::pair<PointTy, PointTy>& C)
  {
    PointTy o1 = oriented_area_2d<PointTy>(A.first,A.second,B.first,B.second,px,py);
    PointTy o2 = oriented_area_2d<PointTy>(B.first,B.second,C.first,C.second,px,py);
    PointTy o3 = oriented_area_2d<PointTy>(C.first,C.second,A.first,A.second,px,py);

    const PointTy zero{0};
    bool pos1 = o1 > zero && !double_cmp(o1, zero);
    bool pos2 = o2 > zero && !double_cmp(o2, zero);
    bool pos3 = o3 > zero && !double_cmp(o3, zero);
    bool neg1 = o1 < zero && !double_cmp(o1, zero);
    bool neg2 = o2 < zero && !double_cmp(o2, zero);
    bool neg3 = o3 < zero && !double_cmp(o3, zero);

    bool has_pos = pos1 || pos2 || pos3;
    bool has_neg = neg1 || neg2 || neg3;

    // if there are strictly negative and strictly positive values => point outside
    return !(has_pos && has_neg);
  };

  // checking the intersection of edges
  using Point_2D = std::pair<PointTy, PointTy>;

  auto edges = [](const Point_2D &A, const Point_2D &B, const Point_2D &C)
  {
      return std::array<std::pair<Point_2D, Point_2D>, 3> { std::pair{A, B},
                                                            std::pair{B, C},
                                                            std::pair{C, A} };
  };

  auto seg_intersect = [&](const Point_2D &A, const Point_2D &B,
                           const Point_2D &C, const Point_2D &D)
  {
      return segments_intersect_2d<PointTy>(A.first, A.second,
                                            B.first, B.second,
                                            C.first, C.second,
                                            D.first, D.second);
  };

  auto e1 = edges(a1, b1, c1);
  auto e2 = edges(a2, b2, c2);

  for (const auto& [p, q] : e1)
  {
      for (const auto& [r, s] : e2)
      {
          if (seg_intersect(p, q, r, s))
              return true;
      }
  }

  // check the nesting, that is, the vertex of one triangle. is inside or on the border of another
  if (point_inside_triangle_2d(a1.first, a1.second, a2, b2, c2)) return true;
  if (point_inside_triangle_2d(a2.first, a2.second, a1, b1, c1)) return true;

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_line_in_3D(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2)
{
  Line<PointTy> line = get_line_from_triangle(t2); // from a degenerate triangle we get a segment

  Vector<PointTy> e1 = vector_from_point(t1.get_b() - t1.get_a());
  Vector<PointTy> e2 = vector_from_point(t1.get_c() - t1.get_a());

  Vector<PointTy> p  = cross(line.vector, e2);
         PointTy det = dot(p, e1);

  if (double_cmp(det, PointTy{0}))
  {
    // we take two different points of the degenerate triangle t2 as the ends of the segment
    Point<PointTy> p = t2.get_a();
    Point<PointTy> q = t2.get_b();

    // if the first two match
    if (p == q)
      q = t2.get_c();

    return intersect_triangle_with_segment_in_2D(t1, line, p, q);
  }

  Vector<PointTy> s = vector_from_point(line.point - t1.get_a());
         PointTy  u = dot(s, p) / det;
  if (u < PointTy{0} || u > PointTy{1})
    return false;

  Vector<PointTy> q = cross(s, e1);
        PointTy   v = dot(line.vector, q) / det;
  if (v < PointTy{0} || v > PointTy{1})
    return false;

  PointTy t = dot(e2, q) / det;
  if (t < PointTy{0} || t > PointTy{1})
    return false;

  if ((u + v) > PointTy{1})
    return false;

  return true;
}

template <typename PointTy = double>
bool intersect_triangle_with_point(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2)
{
  return t1.is_point_in_triangle(t2.get_a());
}

template <typename PointTy = double>
bool check_two_segments_intersection(PointTy min1, PointTy max1,
                                     PointTy min2, PointTy max2)
{
  if (max1 < min2 || min1 > max2)
    return false;

  if (double_cmp(min1, min2) && double_cmp(min1, max1) &&
      double_cmp(min1, max2))
  {
    return false;
  }

  return true;
}


template <typename PointTy = double>
bool intersect_line_with_line(const Triangle<PointTy> &t1, const Triangle<PointTy> &t2)
{
  Line<PointTy> line1 = get_line_from_triangle(t1);
  Line<PointTy> line2 = get_line_from_triangle(t2);

  Point<PointTy> point = line1.intersect_line_with_line(line2);

  auto [min1, max1] = t1.get_triangle_space();
  auto [min2, max2] = t2.get_triangle_space();

  if (!point.valid())
  {
    if (line1.equal(line2))
    {
      if (check_two_segments_intersection(min1.x, max1.x, min2.x, max2.x) ||
          check_two_segments_intersection(min1.y, max1.y, min2.y, max2.y) ||
          check_two_segments_intersection(min1.z, max1.z, min2.z, max2.z))
      {
        return true;
      }
    }

    return false;
  }

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}


template <typename PointTy = double>
bool check_if_inter_point_belongs_sides(const Point<PointTy> &a1, const Point<PointTy> &b1,
                                        const Point<PointTy> &a2, const Point<PointTy> &b2,
                                        const Point<PointTy> &point)
{
  if (!point.valid())
    return false;

  auto [min1, max1] = get_segment_space(a1, b1);
  auto [min2, max2] = get_segment_space(a2, b2);

  return check_if_inter_point_belongs_space(min1, max1, min2, max2, point);
}

template <typename PointTy = double>
bool check_if_inter_point_belongs_space(const Point<PointTy> &min1, const Point<PointTy> &max1, const Point<PointTy> &min2, const Point<PointTy> &max2, const Point<PointTy> &point)
{
  auto is_within_bounds = [](const Point<PointTy> &min,
                             const Point<PointTy> &max,
                             const Point<PointTy> &pt)
  {
    return (pt.x >= min.x && pt.x <= max.x ||
            (double_cmp(pt.x, min.x)   &&
             double_cmp(pt.x, max.x))) &&

           (pt.y >= min.y && pt.y <= max.y ||
            (double_cmp(pt.y, min.y) &&
             double_cmp(pt.y, max.y))) &&

           (pt.z >= min.z && pt.z <= max.z ||
            (double_cmp(pt.z, min.z) &&
             double_cmp(pt.z, max.z)));
  };

  return is_within_bounds(min1, max1, point) &&
         is_within_bounds(min2, max2, point);
}


template <typename PointTy = double>
bool intersect_triangle_with_segment_in_2D(const Triangle<PointTy> &t, const Line<PointTy> &line, const Point<PointTy> &seg_min, const Point<PointTy> &seg_max)
{
  Line <PointTy> line1{vector_from_point(t.get_b() - t.get_a()), t.get_a()};
  Point<PointTy> point1 = line.intersect_line_with_line(line1);
  if (check_if_inter_point_belongs_sides(t.get_a(), t.get_b(), seg_min, seg_max,point1))
  {
    return true;
  }

  Line <PointTy> line2{vector_from_point(t.get_c() - t.get_a()), t.get_a()};
  Point<PointTy> point2 = line.intersect_line_with_line(line2);
  if (check_if_inter_point_belongs_sides(t.get_a(), t.get_c(), seg_min, seg_max, point2))
  {
    return true;
  }

  Line<PointTy>  line3{vector_from_point(t.get_c() - t.get_b()), t.get_b()};
  Point<PointTy> point3 = line.intersect_line_with_line(line3);
  if (check_if_inter_point_belongs_sides(t.get_b(), t.get_c(), seg_min, seg_max, point3))
  {
    return true;
  }

  return false;
}

template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_segment_space(const Point<PointTy> &a, const Point<PointTy> &b)
{
  auto [min_x, max_x] = std::minmax(a.x, b.x);
  auto [min_y, max_y] = std::minmax(a.y, b.y);
  auto [min_z, max_z] = std::minmax(a.z, b.z);

  Point<PointTy> min_vector{min_x, min_y, min_z};
  Point<PointTy> max_vector{max_x, max_y, max_z};

  return std::make_pair(min_vector, max_vector);
}

} // namespace triangle