#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix {
public:
  uint32_t n, m;
  float *mat;

  uint32_t idx(uint32_t n, uint32_t m) const;
  void size(uint32_t n, uint32_t m);

public:
  matrix();
  matrix(matrix const &m);
  ~matrix();
  matrix(std::vector<std::vector<float>> const &in_mat);
  matrix(float *data, uint32_t n, uint32_t m);

  matrix T();
  matrix dot(matrix const &b);
  matrix dot_sparse(matrix const &b);
  static matrix zeros(uint32_t n, uint32_t m);
  static matrix ones(uint32_t n, uint32_t m);
  static matrix random(uint32_t n, uint32_t m);
  float &operator()(uint32_t const n, uint32_t const m);
  float operator()(uint32_t const n, uint32_t const m) const;
  uint32_t size() const;
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
matrix operator+(matrix const &M, matrix const &m);