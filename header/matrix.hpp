#pragma once

#include <array>
#include <sstream>
#include <stdint.h>
#include <vector>
#include "matrix_generic.hpp"

struct matrix1;

struct matrix : public matrix_generic{
  using matrix_generic::size;
  void size(uint32_t n, uint32_t m);

  static matrix normal_dot(matrix const &a, matrix const &b);

  // perform sparse dot product (if a is more spatse than b then normal else
  // reverse)
  static matrix sparse_dot_normal(matrix const &a, matrix const &b);
  static matrix sparse_dot_reverse(matrix const &a, matrix const &b);


  matrix();
  matrix(matrix const &m);
  matrix(matrix_generic const &m);
  matrix(uint32_t const n, uint32_t const m);
  ~matrix();
  matrix(std::vector<std::vector<float>> const &in_mat);
  matrix(float *data, uint32_t n, uint32_t m);

  matrix T() const;
  matrix dot(matrix const &b) const;
  static matrix zeros(uint32_t n, uint32_t m);
  static matrix ones(uint32_t n, uint32_t m);
  static matrix random(uint32_t n, uint32_t m);
  float &operator()(uint32_t const n, uint32_t const m);
  float operator()(uint32_t const n, uint32_t const m) const;
  std::array<uint32_t, 2> shape() const;
  uint32_t shape(uint32_t const axis) const;

  void operator=(matrix_generic &&m);
  matrix &operator=(matrix_generic const &m);
  void operator=(matrix const &m);
};

std::ostream &operator<<(std::ostream &out, matrix const &M);
