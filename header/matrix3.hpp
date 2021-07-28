#pragma once

#include "matrix_generic.hpp"
#include <sstream>
#include <stdint.h>
#include <vector>

struct matrix;
struct matrix1;

struct matrix3 : public matrix_generic {
  using matrix_generic::size;
  void size(uint32_t z, uint32_t n, uint32_t m);

  matrix3();
  matrix3(matrix3 const &m);
  matrix3(matrix_generic const &m);
  matrix3(uint32_t const y, uint32_t const n, uint32_t const m);
  matrix3(float *data, uint32_t const y, uint32_t const n, uint32_t const m);
  matrix3(std::vector<std::vector<std::vector<float>>> const &in_mat);
  ~matrix3();

  static matrix3 zeros(uint32_t z, uint32_t n, uint32_t m);
  static matrix3 ones(uint32_t z, uint32_t n, uint32_t m);
  static matrix3 random(uint32_t z, uint32_t n, uint32_t m);
  matrix squeeze(uint32_t dim) const;
  // take index of matrix
  float &operator()(uint32_t const z, uint32_t const n, const uint32_t m);
  float operator()(uint32_t const z, uint32_t const n, const uint32_t m) const;
  void operator=(matrix_generic const &m);

  std::array<uint32_t, 3> shape() const;
  uint32_t shape(uint32_t const axis) const;
};

std::ostream &operator<<(std::ostream &out, matrix3 const &M);
