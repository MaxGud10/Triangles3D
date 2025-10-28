#include <gtest/gtest.h>

#include "../include/config.hpp"
#include "../include/triangles.hpp"

// bool double_cmp(double x, double y) { return fabs(x - y) < _epsilon; }

TEST(TriangleWithTriangle, Intersection2D_1) {
  Point t1p1{0.4, -5.0, 0.0};
  Point t1p2{1.0, 2.0, 0.0};
  Point t1p3{5.0, 0.0, 0.0};
  Point t2p1{-1.0, 0.0, 0.0};
  Point t2p2{3.0, 2.0, 0.0};
  Point t2p3{6.0, 2.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_2) {
  Point t1p1{1.0, 0.0, 1.0};
  Point t1p2{1.0, 0.0, 5.0};
  Point t1p3{5.0, 0.0, 4.0};
  Point t2p1{5.0, 0.0, 2.0};
  Point t2p2{2.1, 0.0, 8.0};
  Point t2p3{1.0, 0.0, -1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_3) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.5, 0.0};
  Point t2p2{0.0, 1.5, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_4) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_5) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_6) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.1, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_1) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_2) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 20.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_3) {
  Point t1p1{-1.0, 5.0, 4.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_4) {
  Point t1p1{-1.0, 5.0, 4.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{-1.0, -2.0, -1.0};
  Point t2p2{-1.0, -1.0, -1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_5) {
  Point t1p1{2.0, -3.0, 7.0};
  Point t1p2{1.0, 4.0, 6.0};
  Point t1p3{5.0, 3.0, -1.0};
  Point t2p1{1.0, 1.0, 1.0};
  Point t2p2{1.0, 0.0, 1.0};
  Point t2p3{1.0, -1.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_6) {
  Point t1p1{2.0, -3.0, 4.0};
  Point t1p2{1.0, 4.0, 6.0};
  Point t1p3{5.0, 3.0, -1.0};
  Point t2p1{-1.0, 5.0, 2.0};
  Point t2p2{2.0, -1.0, 4.0};
  Point t2p3{5.0, 0.0, 5.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_7) {
  Point t1p1{2.5, 5.234, -4.234};
  Point t1p2{1.31, 5.01, -1.14};
  Point t1p3{-1.1, -2.4, 5.0};
  Point t2p1{1.5, 2.1, 3.1};
  Point t2p2{-1.5, -5.1, 2.0};
  Point t2p3{4.1, 5.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_8) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_9) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{2.0, 2.0, 2.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_10) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{-2.0, -2.0, -2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{1.0, 1.0, -1.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_11) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{3.0, 4.0, 9.0};
  Point t2p2{9.0, 4.0, 16.0};
  Point t2p3{16.0, 4.0, 6.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_12) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{5.0, 16.0, 16.0};
  Point t2p2{19.0, 3.0, 16.0};
  Point t2p3{3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_13) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{2.0, 17.0, 18.0};
  Point t2p2{9.0, 20.0, 3.0};
  Point t2p3{14.0, 6.0, 14.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_14) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{12.0, 7.0, 6.0};
  Point t2p2{12.0, 16.0, 7.0};
  Point t2p3{13.0, 15.0, 11.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_15) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{12.0, 4.0, 10.0};
  Point t2p2{17.0, 20.0, 13.0};
  Point t2p3{18.0, 10.0, 6.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_16) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{5.0, 5.0, 1.0};
  Point t2p3{5.0, 5.0, 10.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_17) {
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{5.0, 5.0, 1.0};
  Point t2p3{5.0, 5.0, 10.0};
  Point t1p1{0.5, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 0.0};
  Point t1p3{0.0, 0.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_18) {
  Point t1p1{5.0, 3.0, 2.0};
  Point t1p2{1.0, 1.0, 0.0};
  Point t1p3{-2.0, 3.0, 1.0};
  Point t2p1{4.0, 1.0, 4.0};
  Point t2p2{4.0, 7.0, 1.0};
  Point t2p3{3.0, 5.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_19) {
  Point t1p1{5.0, 3.0, 2.0};
  Point t1p2{1.0, 1.0, 0.0};
  Point t1p3{-2.0, 3.0, 1.0};
  Point t2p1{4.0, 1.0, 4.0};
  Point t2p2{3.0, 5.0, 0.0};
  Point t2p3{2.0, 3.0, -1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_1) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_2) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_3) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_4) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.5, -1.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_5) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.5, 0.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection3D_6) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.5, 0.1, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_1) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{0.5, 0.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_2) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{0.0, 0.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_3) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{0.0, 0.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_4) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{0.0, 0.0, 2.1};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_5) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{2.5, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_6) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{2.0, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_7) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{-0.5, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_8) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 0.0, 0.0};
  Point t2p2{-2.0, 0.0, 0.0};
  Point t2p3{0.0, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection2D_9) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{-2.0, 1.0, 0.0};
  Point t2p2{-2.0, 1.0, 0.0};
  Point t2p3{2.5, 1.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithPoint, Intersection_1) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{-0.5, 0.0, 1.0};
  Point t2p2{-0.5, 0.0, 1.0};
  Point t2p3{-0.5, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_2) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{-0.5, 0.0, 2.0};
  Point t2p2{-0.5, 0.0, 2.0};
  Point t2p3{-0.5, 0.0, 2.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_3) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_4) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{0.0, 0.0, 2.0};
  Point t2p2{0.0, 0.0, 2.0};
  Point t2p3{0.0, 0.0, 2.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_point(t1, t2));
}

TEST(LineWithLine, Intersection_1) {
  Point t1p1{0.0, 1.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_2) {
  Point t1p1{0.0, 1.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_3) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2)); 
}

TEST(LineWithLine, Intersection_4) {
  Point t1p1{0.5, 0.5, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_5) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{1.0, 1.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_6) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.0, 2.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_7) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.1, 2.1, 0.0};
  Point t2p3{2.1, 2.1, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithPoint, Intersection_1) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{3.0, 3.0, 3.0};
  Point t2p1{0.5, 0.5, 0.5};
  Point t2p2{0.5, 0.5, 0.5};
  Point t2p3{0.5, 0.5, 0.5};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_point(t1, t2));
}

TEST(LineWithPoint, Intersection_2) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{0.5, 0.5, 0.5};
  Point t2p2{0.5, 0.5, 0.5};
  Point t2p3{0.5, 0.5, 0.5};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_point(t1, t2));
}

TEST(LineWithPoint, Intersection_3) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{3.0, 3.0, 3.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_point(t1, t2));
}

TEST(PointWithPoint, Intersection_1) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{1.0, 1.0, 1.0};
  Point t1p3{1.0, 1.0, 1.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(t1.get_a() == t2.get_a());
}

TEST(PointWithPoint, Intersection_2) {
  Point t1p1{0.0, 0.0, 1.0};
  Point t1p2{0.0, 0.0, 1.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(t1.get_a() == t2.get_a());
}


// --- cases from test1.txt-like snippet (four triangles on z = 0, 0.1, 0.2, 0.3) ---

TEST(TriangleWithTriangle, FromTxt_ParallelPlanes_1) {
  // z = 0
  Point a1{-2.392, 3.14, 0.0};
  Point b1{ 0.588, 1.608, 0.0};
  Point c1{ 0.0,  -1.764, 0.0};
  Triangle t1{a1, b1, c1};

  // z = 0.1
  Point a2{-1.164, 1.028, 0.1};
  Point b2{ 1.624, 3.952, 0.1};
  Point c2{ 0.0,  -2.208, 0.1};
  Triangle t2{a2, b2, c2};

  // параллельные, разные плоскости → пересечения нет
  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, FromTxt_ParallelPlanes_2) {
  // z = 0.1
  Point a2{-1.164, 1.028, 0.1};
  Point b2{ 1.624, 3.952, 0.1};
  Point c2{ 0.0,  -2.208, 0.1};
  Triangle t2{a2, b2, c2};

  // z = 0.2
  Point a3{-0.612, 2.16, 0.2};
  Point b3{ 0.376, 2.856, 0.2};
  Point c3{ 0.0, -3.976, 0.2};
  Triangle t3{a3, b3, c3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t2, t3));
}

TEST(TriangleWithTriangle, FromTxt_ParallelPlanes_3) {
  // z = 0.2
  Point a3{-0.612, 2.16, 0.2};
  Point b3{ 0.376, 2.856, 0.2};
  Point c3{ 0.0, -3.976, 0.2};
  Triangle t3{a3, b3, c3};

  // z = 0.3
  Point a4{-3.584, 3.488, 0.3};
  Point b4{ 0.824, 2.268, 0.3};
  Point c4{ 0.0, -1.792, 0.3};
  Triangle t4{a4, b4, c4};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t3, t4));
}

TEST(TriangleWithTriangle, FromTxt_ParallelPlanes_NonAdjacent) {
  // z = 0
  Point a1{-2.392, 3.14, 0.0};
  Point b1{ 0.588, 1.608, 0.0};
  Point c1{ 0.0,  -1.764, 0.0};
  Triangle t1{a1, b1, c1};

  // z = 0.3
  Point a4{-3.584, 3.488, 0.3};
  Point b4{ 0.824, 2.268, 0.3};
  Point c4{ 0.0, -1.792, 0.3};
  Triangle t4{a4, b4, c4};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t4));
}

// инвариант к перестановке вершин (должно давать тот же результат)
TEST(TriangleWithTriangle, FromTxt_ParallelPlanes_PermutationInvariant) {
  // z = 0
  Triangle t1{
    Point{-2.392, 3.14, 0.0},
    Point{ 0.588, 1.608, 0.0},
    Point{ 0.0,  -1.764, 0.0}
  };

  // z = 0.1 (переставим вершины)
  Triangle t2{
    Point{ 1.624, 3.952, 0.1},
    Point{ 0.0,  -2.208, 0.1},
    Point{-1.164, 1.028, 0.1}
  };

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}



int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}