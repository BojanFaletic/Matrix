#pragma once

#include <array>
#include <sstream>
#include <stdint.h>
#include <vector>

class matrix1;

class matrix {
public:
  uint32_t n, m;
  float *mat;
  static bool approx_zero(float const f);
  // number of zero element in matrix
  uint32_t sparsity_cnt_n, sparsity_cnt_m;

  uint32_t idx(uint32_t n, uint32_t m) const;
  void size(uint32_t n, uint32_t m);

  static matrix normal_dot(matrix const &a, matrix const &b);
  static matrix sparse_dot(matrix const &a, matrix const &b);

  void init_sparsity();

public:
  matrix();
  matrix(matrix const &m);
  ~matrix();
  matrix(std::vector<std::vector<float>> const &in_mat);
  matrix(float *data, uint32_t n, uint32_t m);

  matrix T() const;
  matrix dot(matrix const &b) const;
  matrix dot_sparse(matrix const &b) const;
  static matrix zeros(uint32_t n, uint32_t m);
  static matrix ones(uint32_t n, uint32_t m);
  static matrix random(uint32_t n, uint32_t m);
  float &operator()(uint32_t const n, uint32_t const m);
  float operator()(uint32_t const n, uint32_t const m) const;
  void operator=(matrix const &m);
  bool operator==(matrix const &m) const;
  matrix1 flatten() const;
  uint32_t size() const;
  std::array<uint32_t, 2> shape() const;
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