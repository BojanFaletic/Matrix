#pragma once

#include "matrix3.hpp"
#include <sstream>
#include <stdint.h>
#include <vector>

class matrix;
class matrix1;

class matrix3 {
public:
  uint32_t n, m, z;
  float *mat;

  void size(uint32_t z, uint32_t n, uint32_t m);
  uint32_t idx(uint32_t z, uint32_t n, uint32_t m) const;

public:
  matrix3();
  matrix3(matrix3 const &m);
  ~matrix3();
  matrix3(std::vector<std::vector<std::vector<float>>> const &in_mat);

  static matrix3 zeros(uint32_t z, uint32_t n, uint32_t m);
  static matrix3 ones(uint32_t z, uint32_t n, uint32_t m);
  static matrix3 random(uint32_t z, uint32_t n, uint32_t m);
  matrix1 flatten() const;
  matrix squeeze(uint32_t dim) const;
  // take index of matrix
  float &operator()(uint32_t const z, uint32_t const n, const uint32_t m);
  float operator()(uint32_t const z, uint32_t const n, const uint32_t m) const;
  void operator=(matrix3 const &m);
  uint32_t size() const;
};

std::ostream &operator<<(std::ostream &out, matrix3 const &M);

// scalar operators on matrix
matrix3 operator*(matrix3 const &M, float n);
matrix3 operator/(matrix3 const &M, float n);
matrix3 operator+(matrix3 const &M, float n);
matrix3 operator-(matrix3 const &M, float n);
matrix3 operator*=(matrix3 &M, float n);
matrix3 operator/=(matrix3 &M, float n);
matrix3 operator+=(matrix3 &M, float n);
matrix3 operator-=(matrix3 &M, float n);

// operation on matrix
matrix3 operator*(matrix3 const &M, matrix3 const &C);