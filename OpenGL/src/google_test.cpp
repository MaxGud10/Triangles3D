#include <gtest/gtest.h>

#include "../include/triangles.hpp"

namespace triangle {
bool cmp(double x, double y) { return fabs(x - y) < epsilon_; }

TEST(TriangleWithTriangle, Intersection2D_1) {
  Point t1p1{0.4, -5.0, 0.0};
  Point t1p2{1.0, 2.0, 0.0};
  Point t1p3{5.0, 0.0, 0.0};
  Point t2p1{-1.0, 0.0, 0.0};
  Point t2p2{3.0, 2.0, 0.0};
  Point t2p3{6.0, 2.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_5) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.1, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(check_intersection(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_6) {
  Point t1p1{32.2325, 37.9572, 0.0};
  Point t1p2{61.2708, 37.9572, 0.0};
  Point t1p3{32.2325, 131.146, 0.0};
  Point t2p1{42.9767, 50.6096, 0.0};
  Point t2p2{72.015, 50.6096, 0.0};
  Point t2p3{42.9767, 143.798, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_7) {
  Point t1p1{-3.584, 3.488, 0.3};
  Point t1p2{0.824, 2.268, 0.3};
  Point t1p3{0.0, -1.792, 0.3};
  Point t2p1{-1.792, 1.744, 0.3};
  Point t2p2{0.412, 1.134, 0.3};
  Point t2p3{0.0, -0.896, 0.3};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_8) {
  Point t1p1{21.4883, 118.493, 0.0};
  Point t1p2{50.5266, 118.493, 0.0};
  Point t1p3{21.4883, 211.6812, 0.0};
  Point t2p1{32.2325, 37.9572, 0.0};
  Point t2p2{61.2708, 37.9572, 0.0};
  Point t2p3{32.2325, 131.146, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
}

TEST(LineWithLine, Intersection_2) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(LineWithLine, Intersection_3) {
  Point t1p1{0.5, 0.5, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(LineWithLine, Intersection_4) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{1.0, 1.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(check_intersection(t1, t2));
}

TEST(LineWithLine, Intersection_5) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.0, 2.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
}

TEST(LineWithLine, Intersection_6) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.1, 2.1, 0.0};
  Point t2p3{2.1, 2.1, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_FALSE(check_intersection(t1, t2));
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

  ASSERT_TRUE(check_intersection(t1, t2));
}

//=================================================

TEST(TestClassPoint, TestValid) {
  Point p1{1.0, 2.0, 3.0};
  Point<float> p2{NAN, 2.0, 3.0};

  EXPECT_EQ(p1.x, 1.0);
  EXPECT_EQ(p1.y, 2.0);
  EXPECT_EQ(p1.z, 3.0);
  EXPECT_EQ(valid(p1), 1);
  EXPECT_EQ(valid(p2), 0);
}

TEST(TestClassPoint, TestOperations) {
  Point p1{1.0, 2.0, 3.0};
  Point p2{-1.0, 0.0, 1.0};
  Point p3{p1};

  EXPECT_EQ(point_from_vector(p1 - p2), Point(2.0, 2.0, 2.0));
  EXPECT_EQ(p1 - p2, Vector(2.0, 2.0, 2.0));
  EXPECT_EQ(equal(p1, p1), 1);
  EXPECT_EQ(equal(p3, p1), 1);
  EXPECT_EQ(equal(p1, p2), 0);
}

TEST(TestClassVector, TestValid) {
  Point p1{1.0, 2.0, 3.0};
  Point p2{-1.0, 0.0, 1.0};
  Vector v1{1.0, 2.0, 3.0};
  Vector v2{p1 - p2};

  EXPECT_EQ(v1.x, 1.0);
  EXPECT_EQ(v1.y, 2.0);
  EXPECT_EQ(v1.z, 3.0);
  EXPECT_EQ(valid(v1), 1);
  EXPECT_EQ(valid(v2), 1);
}

TEST(TestClassVector, TestOperations) {
  Point p1{1.0, 2.0, 3.0};
  Point p2{-1.0, 0.0, 1.0};
  Vector v1{1.0, 2.0, 3.0};
  Vector v2{-1.0, 0.0, 1.0};
  Vector v3 = v1;
  Vector v4{p1 - p2};

  EXPECT_EQ(v1 == v1, 1);
  EXPECT_EQ(v1 == v2, 0);
  EXPECT_EQ(v1 == v3, 1);
  EXPECT_EQ(v1 == v4, 0);

  EXPECT_EQ(dot(v1, v2), 2);
  EXPECT_EQ(cross(v1, v2), Vector(2.0, -4.0, 2.0));
}

TEST(TestClassLine, TestValid) {
  Point point{1.0, 2.0, 3.0};
  Vector vector{4.0, 5.0, 6.0};

  Line line(vector, point);
  EXPECT_EQ(line.valid(), 1);
}

TEST(TestClassLine, TestOperations) {
  Line line1{Vector(1.0, 0.0, 1.0), Point(0.0, 0.0, 0.0)};
  Line line2{Vector(1.0, 0.0, 1.0), Point(1.0, 0.0, 1.0)};
  Line line3{Vector(4.0, 0.0, 4.0), Point(-1.0, -1.0, -1.0)};
  Line line4{Vector(1.0, 2.0, 3.0), Point(4.0, 5.0, 6.0)};
  Line line5{Vector(2.0, 0.0, 2.0), Point(-1.0, 0.0, -1.0)};

  EXPECT_EQ(line1 == line1, 1);
  EXPECT_EQ(line1 == line2, 1);
  EXPECT_EQ(line1 == line3, 0);
  EXPECT_EQ(line1 == line4, 0);
  EXPECT_EQ(line1 == line5, 1);
}

TEST(TestClassPlane, TestOperations) {
  Point point1{52.0, 52.0, 52.0};
  Point point2{1.0, 0.0, 1.0};

  Plane<double> plane1{0, 0, 0, 1, 0, 0, 0, 1, 0};
  Plane<double> plane2{2, 0, -1, 2, 0, 1, 2, 1, 0};

  EXPECT_EQ(plane1.get_normal(), Vector(0.0, 0.0, 1.0));
  EXPECT_EQ(plane2.get_normal(), Vector(-2.0, 0.0, 0.0));

  EXPECT_EQ(plane1 == plane1, 1);
  EXPECT_EQ(plane1 == plane2, 0);
  EXPECT_EQ(planes_are_parallel(plane1, plane2), 0);
  EXPECT_EQ(planes_are_parallel(plane2, plane2), 1);

  EXPECT_EQ(plane1.substitute(point1), 52);
  EXPECT_EQ(plane2.substitute(point2), 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // namespace triangle