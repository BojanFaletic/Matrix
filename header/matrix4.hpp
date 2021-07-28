#pragma once

#include "matrix_generic.hpp"
#include <sstream>
#include <stdint.h>
#include <vector>

struct matrix4 : public matrix_generic {
  using matrix_generic::size;

  void size(uint32_t y, uint32_t z, uint32_t n, uint32_t m);
  matrix4();
  matrix4(matrix4 const &m);
  matrix4(matrix_generic const &m);
  matrix4(uint32_t const z, uint32_t const y, uint32_t const n,
          uint32_t const m);
  matrix4(float *data, uint32_t const z, uint32_t const y, uint32_t const n,
          uint32_t const m);
  matrix4(
      std::vector<std::vector<std::vector<std::vector<float>>>> const &in_mat);
  ~matrix4();

  static matrix4 zeros(uint32_t y, uint32_t z, uint32_t n, uint32_t m);
  static matrix4 ones(uint32_t y, uint32_t z, uint32_t n, uint32_t m);
  static matrix4 random(uint32_t y, uint32_t z, uint32_t n, uint32_t m);

  // take index of matrix
  float &operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                    uint32_t const m);
  float operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                   uint32_t const m) const;
  void operator=(matrix_generic const &m);

  std::array<uint32_t, 4> shape() const;
  uint32_t shape(uint32_t const axis) const;
};

std::ostream &operator<<(std::ostream &out, matrix4 const &M);