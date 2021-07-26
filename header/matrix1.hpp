#pragma once

#include "matrix1.hpp"
#include "matrix_generic.hpp"
#include <sstream>
#include <stdint.h>
#include <vector>

class matrix;

class matrix1 : public matrix_generic {

  uint32_t idx(uint32_t m) const;

public:
  matrix1();
  matrix1(matrix1 &&m);
  matrix1(matrix1 const &m);
  ~matrix1();
  matrix1(std::vector<float> const &in_mat);

  static matrix1 zeros(uint32_t m);
  static matrix1 ones(uint32_t m);
  static matrix1 random(uint32_t m);
  matrix unsqueeze(uint32_t dim) const;
  // take index of matrix
  float &operator()(uint32_t const m);
  float operator()(uint32_t const m) const;
  void operator=(matrix_generic const &m);
  uint32_t size() const;
  void size(uint32_t m);
};

std::ostream &operator<<(std::ostream &out, matrix1 const &M);
