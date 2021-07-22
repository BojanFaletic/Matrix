#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix3 {
public:
  uint32_t n, m, z;
  float *mat;

  void size(uint32_t y, uint32_t x, uint32_t z);

public:
  matrix3();
  matrix3(matrix3 const &m);
  ~matrix3();
  matrix3(std::vector<std::vector<std::vector<float>>> const &in_mat);

  static matrix3 zeros(uint32_t y, uint32_t x, uint32_t z);
  static matrix3 ones(uint32_t y, uint32_t x, uint32_t z);
  static matrix3 random(uint32_t y, uint32_t x, uint32_t z);
  // take index of matrix
  float &operator()(uint32_t const n, uint32_t const m, const uint32_t z);
  float operator()(uint32_t const n, uint32_t const m, const uint32_t z) const;
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