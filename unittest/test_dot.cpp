#include "matrix.hpp"
#include <cmath>
#include <gtest/gtest.h>

void dot(matrix const &a, matrix const &b, matrix c) { c = a.dot(b); }
void dot_sparse(matrix const &a, matrix const &b, matrix c) {
  c = a.dot_sparse(b);
}

TEST(matrix, dot) {
  const matrix a = matrix::ones(16, 16) * 2;
  const matrix b = matrix::ones(16, 16) * 4;
  const matrix c_correct = matrix::ones(16, 16) * 128;
  matrix c;

  dot(a, b, c);
  EXPECT_EQ(c, c_correct);
}

TEST(matrix, dot_sparse) {
  const matrix a = matrix::ones(16, 16) * 2;
  const matrix b = matrix::ones(16, 16) * 4;
  const matrix c_correct = matrix::ones(16, 16) * 128;
  matrix c;

  dot_sparse(a, b, c);
  EXPECT_EQ(c, c_correct);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}