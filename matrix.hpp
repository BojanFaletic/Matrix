#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix {
public:
  uint32_t n, m;
  bool is_transposed;
  std::vector<float> mat;

  float el(int y, int x) const;

public:
  matrix();
  matrix(std::vector<std::vector<float>> const &in_mat);

  matrix T();
  matrix dot(matrix const &b);
  static matrix zeros(int y, int x);
  static matrix ones(int y, int x);
};

std::ostream &operator<<(std::ostream &out, matrix const &M);

// scalar operators on matrix
matrix operator*(matrix const &M, float n);
matrix operator/(matrix const &M, float n);
matrix operator+(matrix const &M, float n);
matrix operator-(matrix const &M, float n);
matrix operator*=(matrix &M, float n);
matrix operator/=(matrix &M, float n);
matrix operator+=(matrix &M, float n);
matrix operator-=(matrix &M, float n);

// operation on matrix
matrix operator*(matrix const &M, matrix const &C);