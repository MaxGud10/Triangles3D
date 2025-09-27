#include <gtest/gtest.h>

#include "../include/triangles.hpp"
#include "../include/config.hpp"

bool double_cmp(double x, double y) { return fabs(x - y) < epsilon; }

TEST(PointsEqualTest, Test1) 
{

  Point<double> point1(1.0, 2.0, 3.0);
  Point<double> point2(1.0, 2.0, 3.0);

  bool result = (point1 == point2);
  EXPECT_TRUE(result);
}

TEST(PointsEqualTest, Test2) 
{
  Point<double> point1(2.0, 2.0, 3.0);
  Point<double> point2(1.0, 2.0, 3.0);

  bool result = (point1 == point2);
  EXPECT_FALSE(result);
}

TEST(PointsEqualTest, Test3) 
{
  Point<double> point1(1.0000001, 2.0, 3.0);
  Point<double> point2(1.0000002, 2.0, 3.0);

  bool result = (point1 == point2);
  EXPECT_TRUE(result);
}

TEST(SubstitutePointInPlane, Test1) 
{
  Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

  bool result = (plane.substitute(0, 0, 29) == 0);
  EXPECT_TRUE(result);
}

TEST(SubstitutePointInPlane, Test2) 
{
  Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

  bool result = (plane.substitute(0, 0, 30) > 0);
  EXPECT_TRUE(result);
}

TEST(SubstitutePointInPlane, Test3) 
{
  Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

  bool result = (plane.substitute(0, 0, 28) < 0);
  EXPECT_TRUE(result);
}

TEST(SubstitutePointInPlane, Test4) 
{
  Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

  bool result = (plane.substitute(0, 0, -5) >= 0);
  EXPECT_FALSE(result);
}

TEST(PlanesParallel, Test1) 
{
  Plane<double> plane1(1, 0, 4, 0, 1, 4, 0, -1, 4);
  Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

  bool result = planes_are_parallel(plane1, plane2);
  EXPECT_TRUE(result);
}

TEST(PlanesParallel, Test2) 
{
  Plane<double> plane1(1, 0, 0, 0, 1, 0, 0, -1, 0);
  Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

  bool result = planes_are_parallel(plane1, plane2);
  EXPECT_TRUE(result);
}

TEST(PlanesParallel, Test3) 
{
  Plane<double> plane1(1, 0, 4, 0, 1, 1, 0, -1, 2);
  Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

  bool result = planes_are_parallel(plane1, plane2);
  EXPECT_FALSE(result);
}

TEST(PlanesEqualTest, Test1) 
{
  Plane<double> plane1(1, 0, 0, 0, 1, 0, 0, -1, 0);
  Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

  bool result = (plane1 == plane2);
  EXPECT_TRUE(result);
}

TEST(PlanesEqualTest, Test2) 
{
  Plane<double> plane1(1, 0, 2, 0, 1, 0, 0, -1, 0);
  Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

  bool result = (plane1 == plane2);
  EXPECT_FALSE(result);
}

TEST(PlanesEqualTest, Test3) 
{
  Plane<double> plane1(1, 0, 4, 0, 1, 4, 0, -1, 4);
  Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

  bool result = (plane1 == plane2);
  EXPECT_FALSE(result);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(SignedDistTest, Test1) 
{
  Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
  Plane<double> plane2(2, 1, 2.5, 2, 0, 1.5, 2, 1, 0.5);

  double signed_dist1 = plane1.substitute(2, 1, 2.5);
  double signed_dist2 = plane1.substitute(2, 0, 1.5);
  double signed_dist3 = plane1.substitute(2, 1, 0.5);

  bool result = true;
  if ((signed_dist1 < 0 && signed_dist2 < 0 && signed_dist3 < 0) ||
      (signed_dist1 > 0 && signed_dist2 > 0 && signed_dist3 > 0)) 
  {
    result = false;
  }

  EXPECT_FALSE(result);
}

TEST(SignedDistTest, Test2) 
{
  Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
  Plane<double> plane2(2, 0, -1, 2, 0, 1, 2, 1, 0);

  double signed_dist11 = plane1.substitute(2, 1, -1);
  double signed_dist12 = plane1.substitute(2, 0, 1);
  double signed_dist13 = plane1.substitute(2, 1, 0);
  double signed_dist21 = plane2.substitute(0, 0, 0);
  double signed_dist22 = plane2.substitute(1, 0, 0);
  double signed_dist23 = plane2.substitute(0, 1, 0);

  bool result1 = true;
  bool result2 = true;
  bool result;

  if ((signed_dist11 < 0 && signed_dist12 < 0 && signed_dist13 < 0) ||
      (signed_dist11 > 0 && signed_dist12 > 0 && signed_dist13 > 0)) 
  {
    result1 = false;
  }
  if ((signed_dist21 < 0 && signed_dist22 < 0 && signed_dist23 < 0) ||
      (signed_dist21 > 0 && signed_dist22 > 0 && signed_dist23 > 0))     
  {
    result2 = false;
  }

  result = (result1 == true) && (result2 == false);

  EXPECT_TRUE(result);
}

TEST(SignedDistTest, Test3) 
{
  Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
  Plane<double> plane2(1, 1, -1, 1, 1, 1, 0.8, 0.8, 0);

  double signed_dist11 = plane1.substitute(1, 1, -1);
  double signed_dist12 = plane1.substitute(1, 1, 1);
  double signed_dist13 = plane1.substitute(0.8, 0.8, 0);
  double signed_dist21 = plane2.substitute(0, 0, 0);
  double signed_dist22 = plane2.substitute(1, 0, 0);
  double signed_dist23 = plane2.substitute(0, 1, 0);

  bool result1 = true;
  bool result2 = true;
  bool result;

  if ((signed_dist11 < 0 && signed_dist12 < 0 && signed_dist13 < 0) ||
      (signed_dist11 > 0 && signed_dist12 > 0 && signed_dist13 > 0)) 
  {
    result1 = false;
  }
  
  if ((signed_dist21 < 0 && signed_dist22 < 0 && signed_dist23 < 0) ||
      (signed_dist21 > 0 && signed_dist22 > 0 && signed_dist23 > 0)) 
  {
    result2 = false;
  }

  result = (result1 == true) && (result2 == true);

  EXPECT_TRUE(result);
}