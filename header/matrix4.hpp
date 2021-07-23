#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix4 {
public:
  uint32_t n, m, z, y;
  float *mat;

  void size(uint32_t n, uint32_t m, uint32_t z, uint32_t y);
  uint32_t idx(uint32_t n, uint32_t m, uint32_t z, uint32_t y) const;

public:
  matrix4();
  matrix4(matrix4 const &m);
  ~matrix4();
  matrix4(
      std::vector<std::vector<std::vector<std::vector<float>>>> const &in_mat);

  static matrix4 zeros(uint32_t n, uint32_t m, uint32_t z, uint32_t y);
  static matrix4 ones(uint32_t n, uint32_t m, uint32_t z, uint32_t y);
  static matrix4 random(uint32_t n, uint32_t m, uint32_t z, uint32_t y);
  // take index of matrix
  float &operator()(uint32_t const n, uint32_t const m, uint32_t const z,
                    uint32_t const y);
  float operator()(uint32_t const n, uint32_t const m, uint32_t const z,
                   uint32_t const y) const;
  uint32_t size() const;
};

std::ostream &operator<<(std::ostream &out, matrix4 const &M);

// scalar operators on matrix
matrix4 operator*(matrix4 const &M, float n);
matrix4 operator/(matrix4 const &M, float n);
matrix4 operator+(matrix4 const &M, float n);
matrix4 operator-(matrix4 const &M, float n);
matrix4 operator*=(matrix4 &M, float n);
matrix4 operator/=(matrix4 &M, float n);
matrix4 operator+=(matrix4 &M, float n);
matrix4 operator-=(matrix4 &M, float n);
// operation on matrix
matrix4 operator*(matrix4 const &M, matrix4 const &C);