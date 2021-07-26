#include "matrix.hpp"
#include <cmath>
#include <gtest/gtest.h>

void dot(matrix const &a, matrix const &b, matrix &c) { c = a.dot(b); }

TEST(matrix, dot) {
  const matrix a = matrix::ones(2, 4) * 2;
  const matrix b = matrix::ones(4, 2) * 4;
  const matrix c_correct = matrix::ones(2, 2) * 32;
  matrix c;

  dot(a, b, c);
  EXPECT_EQ(c, c_correct);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}